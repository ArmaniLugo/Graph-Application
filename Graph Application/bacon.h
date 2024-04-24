// bacon.h
// this includes and interfaces with the Boost library
// to navigate the graph
// and call certain algorithms 

#pragma once
// regular libraries
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

// boost stuff
#include <boost/config.hpp>
#include <boost/tokenizer.hpp>// this is for processing the input file, breaking up the "actor1;movie;actor2" fields
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>

using namespace std;
using namespace boost;

// boost uses a visitor class in its graph algorithms
// the demo goes over this
// i needed one for BFS, and another for DFS and A*

// a custom class called "bfs_bacon_number_recorder"
template <typename DistanceMap> // this lets us make BCNs with difference types
class bfs_bacon_number_recorder : public default_bfs_visitor // making our own version of default_bfs
{
public:
	bfs_bacon_number_recorder(DistanceMap dist) : d(dist) { }// ctor

	// edges from our graph: in our case, edges are movies
	template <typename Edge, typename Graph>
	void tree_edge(Edge e, const Graph& g) const
	{
		typename graph_traits<Graph>::vertex_descriptor
			u = source(e, g), v = target(e, g);
		d[v] = d[u] + 1;
	}
private:
	DistanceMap d;
};

template <typename DistanceMap> // this lets us make BCNs with difference types
class dfs_bacon_number_recorder : public default_dfs_visitor // making our own dfs version
{
public:
	dfs_bacon_number_recorder(DistanceMap dist) : d(dist) { }// ctor

	// edges from our graph: in our case, edges are movies
	template <typename Edge, typename Graph>
	void tree_edge(Edge e, const Graph& g) const
	{
		typename graph_traits<Graph>::vertex_descriptor
			u = source(e, g), v = target(e, g);
		d[v] = d[u] + 1;
	}
private:
	DistanceMap d;
};

// Convenience function
template < typename DistanceMap >
bfs_bacon_number_recorder<DistanceMap>
record_bacon_number(DistanceMap d)
{
	return bfs_bacon_number_recorder < DistanceMap >(d);
}

// this section is defining some names like 'Graph' to be easier to work with the boost library
// these are aliases for the actual Boost classes
// https://www.boost.org/doc/libs/1_62_0/libs/graph/doc/kevin_bacon.html
typedef adjacency_list < vecS, vecS, undirectedS, property < vertex_name_t,
	std::string >, property < edge_name_t, std::string > > Graph;
typedef property_map < Graph, vertex_name_t >::type actor_name_map_t;
typedef property_map < Graph, edge_name_t >::type movie_name_map_t;
typedef graph_traits < Graph >::vertex_descriptor Vertex;
typedef std::map < std::string, Vertex > NameVertexMap;

 
// note: names to find were hardcoded in because of some const issues
void my_depth_first_search_wrapper(Graph& g, NameVertexMap& actors, std::vector < int >& bacon_number);

void my_A_star_search_wrapper(Graph& g, NameVertexMap& actors, std::vector < int >& bacon_number);
