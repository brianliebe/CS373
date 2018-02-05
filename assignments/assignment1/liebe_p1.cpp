#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>

using namespace std;

struct state {
	int start;
	int accept;
	int state;
};

struct transition {
	int source;
	string value;
	int destination;
};

int main(int argc, char **argv) {
	ifstream input;
	string key;
	// First check to make sure the inputs are valid
	if (argc > 1) {
		input = ifstream(argv[1]);
		if (argc > 2) {
			key = string(argv[2]);
		}
		else {
			cout << "Input string for FA required." << endl;
			return 0;
		}
	}
	else {
		cout << "Input file containing states/transitions required." << endl;
		return 0;
	}

	// The vectors we'll use
	vector<struct state> states;
	vector<pair<struct state, string>> queue;
	vector<struct transition> transitions;
	vector<int> accepted;
	vector<int> rejected;

	// Read each line of the file, and construct a transition or state
	string line;
	while (getline(input, line)) {
		string data;
		stringstream linestream(line);
		getline(linestream, data, '\t');
		if (!data.compare("state")) {
			// Make an empty state
			struct state temp_state;
			temp_state.accept = 0;
			temp_state.start = 0;
			temp_state.state = -1;
			// Fill the state with the data from the file
			while (getline(linestream, data, '\t')) {
				if (!data.compare("start")) temp_state.start = 1;
				else if (!data.compare("accept")) temp_state.accept = 1;
				else temp_state.state = atoi(data.c_str());
			}
			// Add this new state to the execution queue if it's the starter, and the list of states always
			states.push_back(temp_state);
			if (temp_state.start) queue.push_back(make_pair(temp_state, key));
		}
		else {
			// Make an empty transition, and populate it
			struct transition temp_transition;
			getline(linestream, data, '\t');
			temp_transition.source = atoi(data.c_str());
			getline(linestream, data, '\t');
			temp_transition.value = data;
			getline(linestream, data, '\t');
			temp_transition.destination = atoi(data.c_str());
			transitions.push_back(temp_transition);
		}
	}
	
	// The execution code, which will keep running until all possible states are reached
	while (queue.size()) {
		// Take the last state in the queue, and pop it
		pair<struct state, string> current_state = queue.at(queue.size() - 1);
		queue.pop_back();

		// If we've run out of characters from the string, we're done with this state
		if (!current_state.second.compare("")) {
			if (current_state.first.accept == 1) {
				// If it's an accept state, add it to that vector
				int found = 0;
				for (unsigned int i = 0; i < accepted.size(); i++) {
					if (accepted[i] == current_state.first.state) found = 1;
				}
				if (!found) accepted.push_back(current_state.first.state);
			}
			else {
				// If it's not an accept state, add it to the rejects
				int found = 0;
				for (unsigned int i = 0; i < rejected.size(); i++) {
					if (rejected[i] == current_state.first.state) found = 1;
				}
				if (!found) rejected.push_back(current_state.first.state);
			}
		}
		else {
			// Cut the string down by one value
			string transition_value = string(1, current_state.second[0]);
			string new_value = current_state.second;
			new_value.erase(new_value.begin() + 0);

			// Iterate through the possible transitions and look for matches
			for (unsigned int i = 0; i < transitions.size(); i++) {
				if (transitions[i].source == current_state.first.state && transitions[i].value == transition_value) {
					struct state temp_state;
					temp_state.accept = 0;
					temp_state.start = 0;
					temp_state.state = transitions[i].destination;
					// If we already have the state saved, then it's an accept/start, so we should fix those values.
					for (unsigned int j = 0; j < states.size(); j++) {
						if (states[j].state == temp_state.state) {
							temp_state.accept = states[j].accept;
							temp_state.start = states[j].start;
							break;
						}
					}
					// Add this new state to the queue
					queue.push_back(make_pair(temp_state, new_value));
				}
			}
		}
	}
	// Sort the accepted/reject values (not necessary but better to look at).
	sort(accepted.begin(), accepted.end());
	sort(rejected.begin(), rejected.end());
	
	if (accepted.size()) {
		// Print the accept states
		cout << "accept";
		for (unsigned int i = 0; i < accepted.size(); i++) {
			cout << " " << accepted.at(i);
		}
		cout << endl;
	}
	else {
		// Print the reject states
		cout << "reject";
		for (unsigned int i = 0; i < rejected.size(); i++) {
			cout << " " << rejected.at(i);
		}
		cout << endl;
	}
	return 0;
}
