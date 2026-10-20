#include <WiFi.h>
#include <WebServer.h>

// ── pins (adjust if yours differ) ──────────────────────────────────────────
const int PIN_RED   = 21;
const int PIN_GREEN = 19;
const int PIN_BLUE  = 18;

// ── WiFi credentials ───────────────────────────────────────────────────────
const char* SSID     = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// current colour state
uint8_t r = 255, g = 0, b = 0;

// write PWM values to the LED
void applyColour() {
  analogWrite(PIN_RED,   r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE,  b);
}

// ── HTTP handlers ──────────────────────────────────────────────────────────

// GET /colour  →  returns current colour as JSON  {"r":255,"g":0,"b":0}
void handleGetColour() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  String json = "{\"r\":" + String(r) + ",\"g\":" + String(g) + ",\"b\":" + String(b) + "}";
  server.send(200, "application/json", json);
}

// POST /colour  body: r=255&g=0&b=0  or  hex=FF0000
void handleSetColour() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  if (server.hasArg("hex")) {
    String hex = server.arg("hex");
    if (hex.startsWith("#")) hex = hex.substring(1);
    if (hex.length() == 6) {
      r = strtol(hex.substring(0, 2).c_str(), nullptr, 16);
      g = strtol(hex.substring(2, 4).c_str(), nullptr, 16);
      b = strtol(hex.substring(4, 6).c_str(), nullptr, 16);
    }
  } else {
    if (server.hasArg("r")) r = constrain(server.arg("r").toInt(), 0, 255);
    if (server.hasArg("g")) g = constrain(server.arg("g").toInt(), 0, 255);
    if (server.hasArg("b")) b = constrain(server.arg("b").toInt(), 0, 255);
  }

  applyColour();
  String json = "{\"r\":" + String(r) + ",\"g\":" + String(g) + ",\"b\":" + String(b) + "}";
  server.send(200, "application/json", json);
}

// GET /  →  serves the control page (stored in PROGMEM below)
void handleRoot();   // forward-declared; defined after the HTML string

// ── HTML page (stored in flash) ────────────────────────────────────────────
// The page auto-discovers the ESP32's IP from window.location so no
// hardcoding is needed.  It is the same page published as the artifact.
const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<title>RGB Controller</title>
<style>
  /* tokens */
  :root {
    --bg: #0f0f13;
    --surface: #1a1a22;
    --border: #2e2e3e;
    --fg: #e8e8f0;
    --muted: #7a7a96;
    --accent: #6c63ff;
    color-scheme: dark;
  }
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: system-ui, sans-serif;
    background: var(--bg);
    color: var(--fg);
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding: 2rem max(16px, env(safe-area-inset-left,0px));
    gap: 2rem;
  }
  h1 { font-size: 1.4rem; font-weight: 600; letter-spacing: -.02em; }
  .preview {
    width: 140px; height: 140px;
    border-radius: 50%;
    box-shadow: 0 0 60px 10px currentColor;
    transition: background .15s, box-shadow .15s;
  }
  .card {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: 16px;
    padding: 1.5rem;
    width: 100%; max-width: 380px;
    display: flex; flex-direction: column; gap: 1.25rem;
  }
  .picker-row {
    display: flex; gap: 1rem; align-items: center;
  }
  input[type=color] {
    width: 56px; height: 56px;
    border: none; border-radius: 10px;
    cursor: pointer; padding: 2px;
    background: var(--surface);
  }
  .hex-input {
    flex: 1;
    background: var(--bg);
    border: 1px solid var(--border);
    border-radius: 10px;
    color: var(--fg);
    font-size: 1.1rem;
    font-family: monospace;
    padding: .55rem .85rem;
    outline: none;
  }
  .hex-input:focus { border-color: var(--accent); }
  .slider-group { display: flex; flex-direction: column; gap: .7rem; }
  .slider-row { display: grid; grid-template-columns: 1.2rem 1fr 2.5rem; align-items: center; gap: .7rem; }
  .slider-label { font-size: .8rem; font-weight: 700; text-transform: uppercase; }
  .slider-val { font-size: .8rem; font-variant-numeric: tabular-nums; color: var(--muted); text-align: right; }
  input[type=range] {
    -webkit-appearance: none; appearance: none;
    width: 100%; height: 6px; border-radius: 3px;
    outline: none; cursor: pointer;
  }
  input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 18px; height: 18px; border-radius: 50%;
    border: 3px solid var(--surface);
    box-shadow: 0 0 0 1px var(--border);
    cursor: pointer;
  }
  #rSlider { background: linear-gradient(to right,#1a0000,#ff0000); }
  #gSlider { background: linear-gradient(to right,#001a00,#00ff00); }
  #bSlider { background: linear-gradient(to right,#00001a,#0000ff); }
  #rSlider::-webkit-slider-thumb { background: #ff3333; }
  #gSlider::-webkit-slider-thumb { background: #33ff33; }
  #bSlider::-webkit-slider-thumb { background: #3333ff; }
  .presets { display: flex; flex-wrap: wrap; gap: .5rem; }
  .preset {
    width: 36px; height: 36px; border-radius: 50%;
    border: 2px solid transparent;
    cursor: pointer; transition: transform .12s, border-color .12s;
  }
  .preset:hover { transform: scale(1.15); border-color: var(--fg); }
  .send-btn {
    width: 100%;
    background: var(--accent);
    color: #fff;
    font-size: 1rem; font-weight: 600;
    border: none; border-radius: 10px;
    padding: .75rem;
    cursor: pointer;
    transition: opacity .12s;
  }
  .send-btn:hover { opacity: .85; }
  .status {
    font-size: .78rem;
    color: var(--muted);
    text-align: center;
    min-height: 1.2em;
  }
  .status.ok  { color: #4ade80; }
  .status.err { color: #f87171; }
</style>
</head>
<body>

<h1>RGB LED Controller</h1>

<div class="preview" id="preview"></div>

<div class="card">
  <div class="picker-row">
    <input type="color" id="colorPicker" value="#ff0000">
    <input type="text"  id="hexInput" class="hex-input" value="#FF0000" maxlength="7" spellcheck="false">
  </div>

  <div class="slider-group">
    <div class="slider-row">
      <span class="slider-label" style="color:#f87171">R</span>
      <input type="range" id="rSlider" min="0" max="255" value="255">
      <span class="slider-val" id="rVal">255</span>
    </div>
    <div class="slider-row">
      <span class="slider-label" style="color:#4ade80">G</span>
      <input type="range" id="gSlider" min="0" max="255" value="0">
      <span class="slider-val" id="gVal">0</span>
    </div>
    <div class="slider-row">
      <span class="slider-label" style="color:#60a5fa">B</span>
      <input type="range" id="bSlider" min="0" max="255" value="0">
      <span class="slider-val" id="bVal">0</span>
    </div>
  </div>

  <div class="presets" id="presets"></div>

  <button class="send-btn" id="sendBtn">Send to LED</button>
  <p class="status" id="status">Ready</p>
</div>

<script>
const PRESETS = [
  {label:"Red",    hex:"#FF0000"},
  {label:"Green",  hex:"#00FF00"},
  {label:"Blue",   hex:"#0000FF"},
  {label:"White",  hex:"#FFFFFF"},
  {label:"Warm",   hex:"#FF8800"},
  {label:"Cyan",   hex:"#00FFFF"},
  {label:"Magenta",hex:"#FF00FF"},
  {label:"Yellow", hex:"#FFFF00"},
  {label:"Purple", hex:"#8800FF"},
  {label:"Off",    hex:"#000000"},
];

// When served from the ESP32, the ESP32's own IP is window.location.hostname
const BASE = window.location.hostname !== ""
  ? `http://${window.location.hostname}`
  : "";   // standalone artifact preview – no real ESP32

const picker   = document.getElementById("colorPicker");
const hexInput = document.getElementById("hexInput");
const rSlider  = document.getElementById("rSlider");
const gSlider  = document.getElementById("gSlider");
const bSlider  = document.getElementById("bSlider");
const rVal     = document.getElementById("rVal");
const gVal     = document.getElementById("gVal");
const bVal     = document.getElementById("bVal");
const preview  = document.getElementById("preview");
const status   = document.getElementById("status");

function hexToRgb(hex) {
  const h = hex.replace("#","");
  return {
    r: parseInt(h.slice(0,2),16),
    g: parseInt(h.slice(2,4),16),
    b: parseInt(h.slice(4,6),16),
  };
}
function rgbToHex(r,g,b) {
  return "#"+[r,g,b].map(v=>v.toString(16).padStart(2,"0")).join("").toUpperCase();
}

function updateUI(hex) {
  const {r,g,b} = hexToRgb(hex);
  picker.value       = hex.toLowerCase();
  hexInput.value     = hex.toUpperCase();
  rSlider.value      = r; rVal.textContent = r;
  gSlider.value      = g; gVal.textContent = g;
  bSlider.value      = b; bVal.textContent = b;
  preview.style.background   = hex;
  preview.style.color        = hex;
  preview.style.boxShadow    = `0 0 60px 12px ${hex}88`;
}

function syncFromSliders() {
  const hex = rgbToHex(+rSlider.value, +gSlider.value, +bSlider.value);
  updateUI(hex);
}

// events
picker.addEventListener("input",  () => updateUI(picker.value));
[rSlider,gSlider,bSlider].forEach(s => s.addEventListener("input", syncFromSliders));
hexInput.addEventListener("input", () => {
  const v = hexInput.value.trim();
  if (/^#[0-9a-fA-F]{6}$/.test(v)) updateUI(v);
});

// preset buttons
const presetsEl = document.getElementById("presets");
PRESETS.forEach(p => {
  const btn = document.createElement("button");
  btn.className = "preset";
  btn.title = p.label;
  btn.style.background = p.hex;
  btn.addEventListener("click", () => updateUI(p.hex));
  presetsEl.appendChild(btn);
});

// send to ESP32
async function send() {
  const hex = hexInput.value.replace("#","");
  if (BASE === "") {
    status.textContent = "No ESP32 connected (standalone preview)";
    status.className = "status err";
    return;
  }
  status.textContent = "Sending…";
  status.className = "status";
  try {
    const res = await fetch(`${BASE}/colour`, {
      method: "POST",
      headers: {"Content-Type":"application/x-www-form-urlencoded"},
      body: `hex=${hex}`,
    });
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    status.textContent = "✓ Colour set";
    status.className = "status ok";
  } catch(e) {
    status.textContent = `Error: ${e.message}`;
    status.className = "status err";
  }
}

document.getElementById("sendBtn").addEventListener("click", send);

// fetch current colour on load (only when on-device)
if (BASE !== "") {
  fetch(`${BASE}/colour`)
    .then(r => r.json())
    .then(d => updateUI(rgbToHex(d.r, d.g, d.b)))
    .catch(() => {});
}

updateUI("#FF0000");
</script>
</body>
</html>
)rawhtml";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

// ── setup & loop ───────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  applyColour(); // red on boot

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! Open http://");
  Serial.println(WiFi.localIP());

  server.on("/",       HTTP_GET,  handleRoot);
  server.on("/colour", HTTP_GET,  handleGetColour);
  server.on("/colour", HTTP_POST, handleSetColour);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
