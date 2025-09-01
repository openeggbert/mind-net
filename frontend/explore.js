

// ========================================
// ?. Explore
// ========================================
import {API_BASE, apiFetch} from "./api.js";
import {contentArea} from "./dom.js";

let currentCenterNodeId = null;
let parentStack = [];

export async function renderMapExplore(mapId) {
    contentArea.innerHTML = `<div id="network" style="height:600px;border:1px solid #ccc;border-radius:8px;"></div>
    <div style="margin-top:10px;">
      <button id="backBtn" disabled>⬅️ Back</button>
    </div>`;

    const mapJson = await apiFetch(`${API_BASE}/map/${mapId}`);
    if (!mapJson) return;

    if (!currentCenterNodeId) {
        const nodes = new vis.DataSet([
            {id: "map_" + mapJson.id, label: mapJson.name, color: "#1abc9c", level: 0}
        ]);
        const edges = new vis.DataSet([]);
        drawNetwork(nodes, edges, mapId);
        currentCenterNodeId = "map_" + mapJson.id;
        //loadChildren(mapId, currentCenterNodeId);
        await loadTestNodes(mapId, currentCenterNodeId);
    }
}


async function loadChildren(mapId, parentId) {
    let url = `${API_BASE}/note?map_id=${mapId}`;
    if (parentId.startsWith("note_")) {
        const nodeId = parentId.replace("note_", "");
        url += `&parent_note_id=${noteId}`;
    } else {
        // map root notes
        url += `&parent_note_id=0`;
    }
    const json = await apiFetch(url);
    if (!json) return;

    const nodes = network.body.data.nodes;
    const edges = network.body.data.edges;

    json.items.forEach(n => {
        if (!nodes.get("node_" + n.id)) {
            nodes.add({id: "node_" + n.id, label: n.title, color: "#3498db", parentId});

            edges.add({from: parentId, to: "node_" + n.id});
        }
    });
    network.fit();
}

function replaceSpacesWithUnderscores(text) {
    return text.replace(/ /g, "_");
}


function add_node_and_edges(label, color, id_of_parent, level = null) {
    let id_of_new_node = replaceSpacesWithUnderscores(label);
    const nodeData = {id: id_of_new_node, label: label, color: color};
    if (level !== null) nodeData.level = level;  // adds level only if provided
    network.body.data.nodes.add(nodeData);
    network.body.data.edges.add({from: id_of_parent, to: id_of_new_node});
}


async function loadTestNodes(mapId, parentId) {

    add_node_and_edges("ACTIONS", "#8f8484", parentId, 1);
    add_node_and_edges("PARENT", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("CREATE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("READ", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("UPDATE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("DELETE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("History", "#3498db", parentId, 1);
    add_node_and_edges("Basics", "#3498db", parentId, 1);
    add_node_and_edges("Syntax rules", "#3498db", parentId, 1);
    add_node_and_edges("Data types", "#3498db", parentId, 1);
    add_node_and_edges("Operators", "#3498db", parentId, 1);
    add_node_and_edges("OTHER", "#8f8484", parentId, 1);

    add_node_and_edges("content", "#d5db34", "OTHER", 2);
    add_node_and_edges("properties", "#8c5d52", "OTHER", 2);
    add_node_and_edges("tags", "#4cdcbc", "OTHER", 2);
    add_node_and_edges("node_links", "#7e55b3", "OTHER", 2);
    add_node_and_edges("external_links", "#ba6379", "OTHER", 2);

    add_node_and_edges("CREATE.", "#8f8484", "properties", 3);
    add_node_and_edges("colour", "#8c5d52", "properties", 3);
    add_node_and_edges("importance", "#8c5d52", "properties", 3);
    add_node_and_edges("price", "#8c5d52", "properties", 3);


    add_node_and_edges("UPDATE P", "#8f8484", "price", 5);
    add_node_and_edges("DELETE P", "#8f8484", "price", 5);
    add_node_and_edges("key=price", "#8c5d52", "price", 5);
    add_node_and_edges("value=400", "#8c5d52", "price", 5);
    add_node_and_edges("value type=STRING", "#8c5d52", "price", 5);

    network.fit();
}


let network = null;

function drawNetwork(nodes, edges, mapId) {
    const container = document.getElementById('network');
    const data = {nodes, edges};


    let o = 2;
    switch (o) {
        case 1: {
            const options = {physics: {stabilization: true}, interaction: {hover: true}};
            network = new vis.Network(container, data, options);
        }
            break;

        case 2: {
            const options = {
                interaction: {
                    dragNodes: true,
                    zoomView: true,
                    dragView: true,
                    multiselect: true,
                    selectable: true,
                    hover: true,
                    touch: true
                },
                layout: {
                    hierarchical: {
                        direction: "UD",
                        sortMethod: "directed", // nebo 'hubsize'
                        levelSeparation: 80,
                        nodeSpacing: 100
                    }
                },
                physics: {enabled: true}
            };


            network = new vis.Network(container, data, options);
        }
            break;
        case 3: {
            const options = {
                physics: {
                    enabled: true,
                    stabilization: {
                        enabled: true,
                        iterations: 100,
                        updateInterval: 10
                    }
                },
                layout: {
                    improvedLayout: true
                },
                interaction: {
                    hover: true
                }
            };

            network = new vis.Network(container, data, options);

            network.once("stabilizationIterationsDone", () => {
                network.setOptions({physics: false});
            });
        }
    }


    network.on("click", params => {
        if (params.nodes.length > 0) {
            const nodeId = params.nodes[0];
            if (nodeId !== currentCenterNodeId) {
                parentStack.push(currentCenterNodeId);
                currentCenterNodeId = nodeId;
                document.getElementById("backBtn").disabled = false;
                loadChildren(mapId, nodeId);
            }
        }
    });


    document.getElementById("backBtn").onclick = () => {
        if (parentStack.length > 0) {
            currentCenterNodeId = parentStack.pop();
            if (parentStack.length === 0) document.getElementById("backBtn").disabled = true;
            renderMapExplore(1);
        }
    };
}
