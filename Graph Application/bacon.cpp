#include "bacon.h"


void my_depth_first_search_wrapper(Graph& g, NameVertexMap& actors,
	std::vector < int >& bacon_number)
{
	cout << "------------------------------------------\n";
	cout << "DEPTH FIRST SEARCH on Val Kilmer" << endl;
	const std::string new_actor_name = "Val Kilmer"; // "Kevin Bacon
	//std::cin >> new_actor_name;
	if (actors.find(new_actor_name) == actors.end())
	{
		cout << "Actor not found" << endl;
		return;
	}

	size_t vid = actors[new_actor_name];
	Vertex dest = vertex(vid, g);

	cout << "Starting depth first search at " << new_actor_name << endl;
	cout << bacon_number[dest] << endl;

}

void my_A_star_search_wrapper(Graph& g, NameVertexMap& actors, std::vector < int >& bacon_number) {
	cout << "------------------------------------------\n";
	cout << "A* FIRST SEARCH (null heuristic) on A. Paliakov" << endl;
	const std::string new_actor_name = "A. Paliakov"; // "Kevin Bacon
	//std::cin >> new_actor_name;
	if (actors.find(new_actor_name) == actors.end())
	{
		cout << "Actor not found" << endl;
		return;
	}

	size_t vid = actors[new_actor_name];
	Vertex dest = vertex(vid, g);

	cout << "Starting depth first search at " << new_actor_name << endl;
	cout << bacon_number[dest] << endl;

}
