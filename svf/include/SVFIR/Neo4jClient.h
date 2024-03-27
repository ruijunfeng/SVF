//===- Neo4jClient.h -----------------------------------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013->2024>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//

#include <Python.h>
#include "Util/cJSON.h"

#ifndef INCLUDE_NEO4JCLIENT_H_
#define INCLUDE_NEO4JCLIENT_H_

namespace SVF{

class DbNode {
    
private:
    const char* node_type;
    PyObject* node_properties;

public:
    // Constructor
    DbNode(const char* node_type, PyObject* node_properties) {
        // Set the node_type
        this->node_type = node_type;
        // Set the node_properties
        this->node_properties = node_properties;
    }

    // Destructor
    ~DbNode() {
        Py_DECREF(node_properties);
    }

    // Get the node_type
    const char* getNodetype() const {
        return node_type;
    }

    // Get the node_properties
    PyObject* getNodeProperties() const {
        return node_properties;
    }
};

class DbEdge {

private:
    const char* edge_type;
    PyObject* edge_properties;

public:
    // Constructor
    DbEdge(const char* edge_type, PyObject* edge_properties) {
        // Set the edge_type
        this->edge_type = edge_type;
        // Set the edge_properties
        this->edge_properties = edge_properties;
    }

    // Destructor
    ~DbEdge() {
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
    Neo4jClient(const char* uri, const char* username, const char* password);
    // Destructor
    ~Neo4jClient();

    DbNode createNodeJson(const char* graph_id, const char* node_type, cJSON* jsonNode);

    DbNode createNode(const char* graph_id, const char* node_type, ...);

    DbEdge createEdgeJson(const char* graph_id, const char* edge_type, cJSON* edgeJson);

    DbEdge createEdge(const char* graph_id, const char* edge_type, ...);
    
    void writeNode(const DbNode& node);

    void writeEdge(const DbNode& node1, const DbNode& node2, const DbEdge& edge);

    void clearDatabase();
};

}

#endif