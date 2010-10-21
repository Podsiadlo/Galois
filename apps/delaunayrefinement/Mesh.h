/* 
 
 Lonestar DelaunayRefinement: Refinement of an initial, unrefined Delaunay
 mesh to eliminate triangles with angles < 30 degrees, using a
 variation of Chew's algorithm.
 
 Authors: Milind Kulkarni 
 
 Copyright (C) 2007, 2008 The University of Texas at Austin
 
 Licensed under the Eclipse Public License, Version 1.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.eclipse.org/legal/epl-v10.html
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 File: Mesh.h
 
 Modified: February 12th, 2008 by Milind Kulkarni (initial version)
 
 */ 

#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <fstream>
#include <istream>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

/**
 * Helper class used providing methods to read in information and create the graph 
 *
 */
class Mesh {
  // used during reading of the data and creation of the graph, to record edges between nodes of the graph 
  std::map<Edge, GNode> edge_map;

  /**
   * 
   * @param mesh The graph representing the mesh
   * @return the bad triangles in the graph
   */
 public:
  template<typename Collection>
    int getBad(Graph* mesh, Collection& ret) {
    int retval = 0;
    for(Graph::active_iterator ii = mesh->active_begin(), ee = mesh->active_end(); ii != ee; ++ii) {
      Element& element = ii->getData();
      if (element.isBad()) {
	ret.push(*ii);
	++retval;
      }
    }
    return retval;
  }

 private:

  void readNodes(std::string filename, std::vector<Tuple>& tuples) {
    std::ifstream scanner((filename + ".node").c_str());
    int ntups;
    scanner >> ntups;
    int dummy;
    scanner >> dummy >> dummy >> dummy;

    tuples.resize(ntups);
    for (int i = 0; i < ntups; i++) {
      int index;
      double x;
      double y;
      double z;
      scanner >> index >> x >> y >> z;
      tuples[index] = Tuple(x, y, 0);
    }
  }

  void readElements(Graph* mesh, std::string filename, std::vector<Tuple>& tuples) {
    std::ifstream scanner((filename + ".ele").c_str());
    
    int nels;
    int dummy;
    scanner >> nels >> dummy >> dummy;
    for (int i = 0; i < nels; i++) {
      int index;
      int n1;
      int n2;
      int n3;
      scanner >> index >> n1 >> n2 >> n3;
      Element e(tuples[n1], tuples[n2], tuples[n3]);
      addElement(mesh, e);
    }
  }

  void readPoly(Graph* mesh, std::string filename, std::vector<Tuple>& tuples) {
    std::ifstream scanner((filename + ".poly").c_str());
    int dummy;
    scanner >> dummy >> dummy >> dummy >> dummy;
    int nsegs;
    scanner >> nsegs;
    scanner >> dummy;
    for (int i = 0; i < nsegs; i++) {
      int index;
      int n1;
      int n2;
      scanner >> index >> n1 >> n2 >> dummy;
      Element e(tuples[n1], tuples[n2]);
      addElement(mesh, e);
    }
  }
  
  // .poly contains the perimeter of the mesh; edges basically, which is why it contains pairs of nodes
 public:
  void read(Graph* mesh, std::string basename) {
    std::vector<Tuple> tuples;
    readNodes(basename, tuples);
    readElements(mesh, basename, tuples);
    readPoly(mesh, basename, tuples);
  }

 protected:
  GNode addElement(Graph* mesh, Element& element) {
    GNode node = mesh->createNode(element);
    mesh->addNode(node);
    for (int i = 0; i < element.numEdges(); i++) {
      Edge edge = element.getEdge(i);
      if (edge_map.find(edge) == edge_map.end()) {
        edge_map[edge] = node;
      } else {
        mesh->addEdge(node, edge_map[edge], edge);
        edge_map.erase(edge);
      }
    }
    return node;
  }

 public:
  bool verify(Graph* mesh) {
    // ensure consistency of elements
    bool error = false;
     
    for (Graph::active_iterator ii = mesh->active_begin(), ee = mesh->active_end(); ii != ee; ++ii) {
 
      GNode node = *ii;
      Element& element = node.getData();
      if (element.getDim() == 2) {
	if (mesh->neighborsSize(node) != 1) {
	  std::cerr << "-> Segment " << element << " has " << mesh->neighborsSize(node) << " relation(s)\n";
	  error = true;
	}
      } else if (element.getDim() == 3) {
	if (mesh->neighborsSize(node) != 3) {
	  std::cerr << "-> Triangle " << element << " has " << mesh->neighborsSize(node) << " relation(s)";
	  error = true;
	}
      } else {
	std::cerr << "-> Figures with " << element.getDim() << " edges";
	error = true;
      }
    }
    
    if (error)
      return false;
    
    // ensure reachability
    std::stack<GNode> remaining;
    std::set<GNode> found;
    remaining.push(*(mesh->active_begin()));
      
    while (!remaining.empty()) {
      GNode node = remaining.top();
      remaining.pop();
      if (!found.count(node)) {
	assert(mesh->containsNode(node) && "Reachable node was removed from graph");
	found.insert(node);
	for (Graph::neighbor_iterator ii = mesh->neighbor_begin(node), ee = mesh->neighbor_end(node); ii != ee; ++ii)
	  remaining.push(*ii);
      }
    }

    if (found.size() != mesh->size()) {
      std::cerr << "Not all elements are reachable \n";
      std::cerr << "Found: " << found.size() << "\nMesh: " << mesh->size() << "\n";
      assert(0 && "Not all elements are reachable");
      return false;
    }
    return true;
  }
};

#endif
