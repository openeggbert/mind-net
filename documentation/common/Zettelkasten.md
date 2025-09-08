# Zettelkasten

## zettelkasten.html

```aiignore

🧱 HTML + CSS + JS prototyp

<!DOCTYPE html>
<html lang="cs">
<head>
  <meta charset="UTF-8">
  <title>Zettelkasten</title>
  <style>
    body {
      background-color: #f5f5dc; /* béžová */
      font-family: sans-serif;
      padding: 20px;
    }

    .zettel {
      border: 3px solid black;
      padding: 20px;
      background-color: #fffaf0;
      margin-bottom: 20px;
      position: relative;
    }

    .zettel h2 {
      margin-top: 0;
    }

    .zettel textarea {
      width: 100%;
      height: 150px;
      font-family: monospace;
      font-size: 14px;
    }

    .zettel .markdown-view {
      display: none;
      background-color: #fdf6e3;
      padding: 10px;
      border: 1px dashed #aaa;
    }

    .zettel .children {
      margin-top: 20px;
      padding-left: 20px;
      border-left: 2px solid #ccc;
    }

    .zettel button {
      margin-right: 10px;
    }

    .properties-tab {
      display: none;
      background-color: #e0e0e0;
      padding: 10px;
      margin-top: 10px;
    }
  </style>
</head>
<body>

<div class="zettel">
  <h2>Poznámka #123</h2>
  <input type="text" placeholder="Název poznámky" value="Jak funguje RAII">
  <textarea id="content">RAII je idiom v C++ pro správu zdrojů...</textarea>
  <div class="markdown-view" id="markdownContent"></div>

  <button onclick="toggleView()">Přepnout zobrazení</button>
  <button onclick="showProperties()">Show Properties</button>
  <button onclick="deleteNote()">Delete</button>

  <div class="properties-tab" id="propertiesTab">
    <strong>Properties:</strong><br>
    ID: 123<br>
    Parent: 45<br>
    Tags: C++, idiom, memory
  </div>

  <div class="children">
    <h4>Podpoznámky:</h4>
    <ul>
      <li>Poznámka #124 – `unique_ptr` vs `shared_ptr`</li>
      <li>Poznámka #125 – Destruktory a výjimky</li>
    </ul>
  </div>
</div>

<script>
  let isMarkdown = false;

  function toggleView() {
    const textarea = document.getElementById('content');
    const markdownDiv = document.getElementById('markdownContent');

    if (!isMarkdown) {
      markdownDiv.innerHTML = marked.parse(textarea.value);
      markdownDiv.style.display = 'block';
      textarea.style.display = 'none';
    } else {
      markdownDiv.style.display = 'none';
      textarea.style.display = 'block';
    }

    isMarkdown = !isMarkdown;
  }

  function showProperties() {
    const tab = document.getElementById('propertiesTab');
    tab.style.display = tab.style.display === 'none' ? 'block' : 'none';
  }

  function deleteNote() {
    alert("Poznámka bude smazána (simulace)");
  }
</script>

<!-- Markdown parser (např. CDN knihovna) -->
<script src="https://cdn.jsdelivr.net/npm/marked/marked.min.js"></script>

</body>
</html>

```