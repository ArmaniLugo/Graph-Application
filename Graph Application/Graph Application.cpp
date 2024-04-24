
#include "bacon.h"

// My demo is based upon Boost's demo but I've expanded it and added lots of documentation
// https://www.boost.org/doc/libs/1_62_0/libs/graph/doc/kevin_bacon.html

// A. Background
// spend some time talking about kevin bacon being  big deal in the 80s,90s days
// he was in a lot of movise with a lot of actors across many genres
// https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon#:~:text=The%20computation%20of%20a%20Bacon,a%20Bacon%20number%20of%201.
// this a classic graph problem!


// B.  How do we model this?
// 2 parts to a graph: a set of edges and nodes (vertices)
// in our example: 
// vertices = actors, 
// edges = movies
// direct vs indirect connections


// C. building the graph
// get a data file
// each line of the text represents the edge between two vertices
// example: 'William Shatner;Loaded Weapon 1 (1993);Denise Richards' means these 2 actors have 1 degree separation with each other
// but we first have to use boost library to break apart (tokenize) each line
// ...token1='william shatner', token2='Loaded Weapon 1 (1993)', token3='denise richards'
// Not every actor is in a movie with Kevin Bacon    
// 'Denise Richards;Wild Things(1998);Kevin Bacon' will connect KB with WS indirectly through DR

int main()
{
	// open the file
	std::ifstream datafile("C:/Users/Owner/OneDrive/Documents/Python/boost_1_85_0/libs/graph/example/kevin-bacon.dat");
	if (!datafile) {
		std::cerr << "No ./kevin-bacon.dat file" << std::endl;
		return EXIT_FAILURE;
	}

	// these are typedefs (redefinitions, aliases) of Boost classes
	Graph g;
	actor_name_map_t actor_name = get(vertex_name, g);
	movie_name_map_t connecting_movie = get(edge_name, g);
	NameVertexMap actors;

	// process each line of the file using Boost's tokenizer,
	// then connecting them via an edge and adding that pair to a graph
	// Boost has its own way of parsing input files so this gets pretty weird!
	for (std::string line; std::getline(datafile, line);) {// goes line by line in the file 

		// PART A: parsing (break up) each line into tokens

		// example: 'William Shatner;Loaded Weapon 1 (1993);Denise Richards' becomes...
		// ...token1='william shatner', token2='Loaded Weapon 1 (1993)', token3='denise richards'
		char_delimiters_separator < char >sep(false, "", ";");// tokens are separated by the ';' character
		tokenizer <> line_toks(line, sep);
		tokenizer <>::iterator i = line_toks.begin();//for looping over what we just separated
		std::string actors_name = *i++;
		NameVertexMap::iterator pos;
		bool inserted;

		// We need those 2 vertices and their edge to connect them into the map
		// That is, we insert the pair of actors into the map, connnecting them by the movie they both star in

		Vertex u, v;// these are the 2 actors: will shatner and denise richards in our example

		// PART B: adding U to graph
		boost::tie(pos, inserted) = actors.insert(std::make_pair(actors_name, Vertex()));// make vertex with 'William Shatner'
		if (inserted) {
			u = add_vertex(g);// put U into graph
			actor_name[u] = actors_name;
			pos->second = u;
		}
		else
			u = pos->second;

		std::string movie_name = *i++;// this is the name of the connection; the second token

		// PART C: adding V to graph
		boost::tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));
		if (inserted) {
			v = add_vertex(g);//put V into graph
			actor_name[v] = *i;
			pos->second = v;
		}
		else
			v = pos->second;

		// now that graph knows about the 2 actors, we can connect them with an edge


		// PART D: connect the 2 nodes with an edge
		// update our graph with those nodes so our graph knows about the nodes being connected
		graph_traits < Graph >::edge_descriptor e;// we give information to the edge (the movie that connects the vetices/actors)
		boost::tie(e, inserted) = add_edge(u, v, g);// u and v were already connected, now we just update our graph with that connection
		if (inserted)
			connecting_movie[e] = movie_name;

	}

	// VERTEX ID IS AN INDEX THAT WE USE TO FIND THE BACON NUMBER ASSOCIATED WITH THAT VERTEX ID
	// a vector is an array that can grow 
	// we're making an array of all the actors (the function num_vertices(g) tells us how many actors exist in the file)
	// we're calling that array 'bacon_number'-> it's going to hold the bacon_number of each actor
	// if the actor's id is 12, then his Bacon Number is at bacon_number[12]
	std::vector < int >	bacon_number(num_vertices(g));// if 100 actors, we make space for 100 bacon numbers

	Vertex src = actors["Kevin Bacon"];
	bacon_number[src] = 0;// Kevin Bacon is contracturally obligated to always work with Kevin Bacon

	// some notes about data structures:
	// 'actors<const string, vertexID>' is a map from names ('Marilyn Monroe') -> it helps us find an id that we use for bacon number array
	// that bacon number is found by looking at 'bacon_numbers' which is a flat vector of bacon numbers
	// e.g. let's say actors['Armani Lugo'] = 42 then we look at bacon_numbers[42] to find my Kevin Bacon Number (which this file)

	// 1. call BFS on Kevin Bacon to see EVERY actor's distance from Kevin Bacon
	// this searches the graph using kevin bacon as the source node
	// it finds the distance from him to every other actor
	breadth_first_search(g, src,
		visitor(record_bacon_number(&bacon_number[0])));// this is a 'raw' Boost graph call; i wrapped the DFS and A* calls
	graph_traits < Graph >::vertex_iterator i, end;
	for (boost::tie(i, end) = vertices(g); i != end; ++i) {
		std::cout << actor_name[*i] << " has a Bacon number of "
			<< bacon_number[*i] << std::endl;
	}

	// the next 2 algorithms start at an actor (hardcoded in as `val kilmer` and then `A. Paliakov`)

	// #2 DFS algorithm: choose an actor name, and find their KBN
	// we can use the same graph, but we need to reset the bacon_number array
	my_depth_first_search_wrapper(g, actors, bacon_number);// val kilmer

	// #3 A* algorithm: do the same thing but with A*
	my_A_star_search_wrapper(g, actors, bacon_number);// a. paliakov
	return 0;
}

