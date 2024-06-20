#include <Python.h>

class Node {
    
private:
    const char* node_type;
    PyObject* node_properties;

public:
    // Constructor
    Node(const char* node_type, PyObject* node_properties) {
        // Set the node_type
        this->node_type = node_type;
        // Set the node_properties
        this->node_properties = node_properties;
    }

    // Destructor
    ~Node() {
        Py_DECREF(node_properties);
    }

    // Get the nodetype
    const char* getNodetype() const {
        return node_type;
    }

    // Get the properties
    PyObject* getNodeProperties() const {
        return node_properties;
    }
};

class Edge {

private:
    const char* edge_type;
    PyObject* edge_properties;

public:
    // Constructor
    Edge(const char* edge_type, PyObject* edge_properties) {
        // Set the edge_type
        this->edge_type = edge_type;
        // Set the edge_properties
        this->edge_properties = edge_properties;
    }

    // Destructor
    ~Edge() {
        Py_DECREF(edge_properties);
    }

    // Get the edge_type
    const char* getEdgeType() const {
        return edge_type;
    }

    // Get the edge_properties
    PyObject* getEdgeProperties() const {
        return edge_properties;
    }
};

class Neo4jClient {

private:
    PyObject* pInstance;

public:
    // Constructor
    Neo4jClient(const char* uri = "neo4j+s://4291d08d.databases.neo4j.io", const char* username = "neo4j", const char* password = "ghfQKOPyySepbGDVuGgWsJLhhHP2R-ukd3tbvT9QNu8") {
        // Initialize the Python interpreter
        Py_Initialize();

        // Add the path to the Python module (/home/SVF-tools/SVF-json2database/.neo4j-cpp/Neo4jClient.py)
        PyObject* sys = PyImport_ImportModule("sys");
        PyObject* path = PyObject_GetAttrString(sys, "path");
        PyList_Append(path, PyUnicode_FromString("/home/SVF-tools/SVF/.neo4j-cpp"));

        // Import the Python module (Neo4jClient.py)
        PyObject* pModule = PyImport_ImportModule("Neo4jClient");
        if (pModule == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to import module");
            PyErr_Print(); // Print error information
            return; // Failed to import module
        }

        // Get the class definition (Neo4jClient)
        PyObject* pClass = PyObject_GetAttrString(pModule, "Neo4jClient");
        if (pClass == nullptr || !PyCallable_Check(pClass)) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to get class definition");
            PyErr_Print();
            Py_DECREF(pModule); // Clean up pModule
            return; // Class not found or not callable
        }

        // Create an instance of the class
        pInstance = PyObject_CallObject(pClass, Py_BuildValue("(sss)", uri, username, password));
        if (pInstance == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to create class instance");
            PyErr_Print();
            Py_DECREF(pClass); // Clean up pClass
            Py_DECREF(pModule); // Clean up pModule
            return; // Failed to create instance
        }

        // Clean up
        Py_DECREF(pClass);
        Py_DECREF(pModule);
    }

    // Destructor
    ~Neo4jClient() {
        // Close the connection to neo4j database
        PyObject_CallMethod(pInstance, "close", nullptr);

        // Clean up
        Py_DECREF(pInstance);

        // Finalize the Python interpreter
        Py_Finalize();
    }

    Node createNode(const char* graph_id, const char* node_type, ...) {
        // Create a new dictionary to store the node properties
        PyObject* properties = PyDict_New();
        if (properties == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to create node properties dictionary");
            PyErr_Print();
        }

        // Add graph_id to properties dictionary
        PyDict_SetItemString(properties, "graph_id", PyUnicode_FromString(graph_id));

        // Write the key-value pairs to the properties dictionary
        va_list args; // All variable arguments passed from the function call
        va_start(args, node_type); // Start reading the variable arguments after the node_type
        const char* key;
        PyObject* value;
        while ((key = va_arg(args, const char*)) != nullptr) { // Read the key (use nullptr to mark the ends of the variable arguments list)
            PyObject* pyKey = PyUnicode_FromString(key);
            value = va_arg(args, PyObject*); // Read the value
            PyDict_SetItem(properties, pyKey, value); // Add the key-value pair to the properties dictionary
        }
        va_end(args);

        // Create a new Node object and return it
        return Node(node_type, properties);
    }

    Edge createEdge(const char* graph_id, const char* edge_type, ...) {
        // Create a new dictionary to store the edge properties
        PyObject* properties = PyDict_New();
        if (properties == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to create edge properties dictionary");
            PyErr_Print();
        }

        // Add graph_id to properties dictionary
        PyDict_SetItemString(properties, "graph_id", PyUnicode_FromString(graph_id));

        // Write the key-value pairs to the properties dictionary
        va_list args; // All variable arguments passed from the function call
        va_start(args, edge_type); // Start reading the variable arguments after the edge_type
        const char* key;
        PyObject* value;
        while ((key = va_arg(args, const char*)) != nullptr) { // Read the key (use nullptr to mark the ends of the variable arguments list)
            PyObject* pyKey = PyUnicode_FromString(key);
            value = va_arg(args, PyObject*); // Read the value
            PyDict_SetItem(properties, pyKey, value); // Add the key-value pair to the properties dictionary
        }
        va_end(args);

        // Create a new Node object and return it
        return Edge(edge_type, properties);
    }


    void writeNode(const Node& node) {
        // Call create_node method
        PyObject* pResult = PyObject_CallMethod(pInstance, "create_node", "(sO)",
                                                node.getNodetype(), node.getNodeProperties());
        // Check if the call succeeded
        if (pResult == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to call create_node method in python module");
            PyErr_Print();
            return;
        }
    }

    void writeEdge(const Node& node1, const Node& node2, const Edge& edge) {
        // Call create_edge method
        PyObject* pResult = PyObject_CallMethod(pInstance, "create_edge", "(sOsOsO)",
                                                node1.getNodetype(), node1.getNodeProperties(),
                                                node2.getNodetype(), node2.getNodeProperties(),
                                                edge.getEdgeType(), edge.getEdgeProperties());
        // Check if the call succeeded
        if (pResult == nullptr) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to call create_edge method");
            PyErr_Print();
            return;
        }
    }

    void clearDatabase() {
        // Clear neo4j database
        PyObject_CallMethod(pInstance, "clear_database", nullptr);
    }

};

int main() {
    // Initialize the neo4j client
    Neo4jClient client;

    // Clear the neo4j database
    client.clearDatabase();

    const char* graph_id = "0";
    // Create node1
    // Call createNode to create a node object
    Node node1 = client.createNode(graph_id, "Person", "name", PyUnicode_FromString("Bob"), "age", PyLong_FromLong(35), nullptr);
    // Call writeNode to write node1 into database
    client.writeNode(node1);

    // Create node2
    // Call createNode to create a node object
    Node node2 = client.createNode(graph_id, "Person", "name", PyUnicode_FromString("Alice"), "age", PyLong_FromLong(25), nullptr);
    // Call writeNode to write node2 into database
    client.writeNode(node2);

    // Link node1 and node2
    // Call createEdge to create an edge object
    Edge edge = client.createEdge(graph_id, "KNOWS", nullptr);
    // Call writeEdge to write edge into database
    client.writeEdge(node1, node2, edge);
    
    // Close the connection to neo4j database
    // This will be handled by the destructor of the Neo4jClient class

    return 0;
}