#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

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
	ifstream input(argv[1]);
	string key = argv[2]; 
	vector<struct state> states;
	vector<pair<struct state, string>> queue;
	vector<struct state> accepts;
	vector<struct transition> transitions;
	
	string line;
	while (getline(input, line)) {
		string data;
		stringstream linestream(line);
		getline(linestream, data, '\t');
		if (!data.compare("state")) {
			struct state temp_state { temp_state.accept = 0, temp_state.start = 0, temp_state.state = -1 };
			while (getline(linestream, data, '\t')) {
				if (!data.compare("start")) temp_state.start = 1;
				else if (!data.compare("accept")) temp_state.accept = 1;
				else temp_state.state = atoi(data.c_str());
			}
			states.push_back(temp_state);
			if (temp_state.accept) accepts.push_back(temp_state);
			if (temp_state.start) queue.push_back(make_pair(temp_state, key));
		}
		else {
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

	for (unsigned int i = 0; i < states.size(); i++) { 
		cout << "state:" << states[i].state << " acc:" << states[i].accept << " start:" << states[i].start << endl;
	}
	for (unsigned int i = 0; i < transitions.size(); i++) {
		cout << "src:" << transitions[i].source << " val:" << transitions[i].value << " dest:" << transitions[i].destination << endl;
	}

	while (queue.size()) {
		pair<struct state, string> current_state = queue.at(queue.size() - 1);
		string transition_value = string(1, current_state.second[0]);
		string new_value = current_state.second;

		new_value.erase(new_value.begin() + 0);
		queue.pop_back();
		vector<struct transition> state_transitions;
		
		for (unsigned int i = 0; i < transitions.size(); i++) {
			if (transitions[i].source == current_state.first.state && transitions[i].value == transition_value) {
				struct state temp_state { temp_state.state = transitions[i].destination, temp_state.accept = 0, temp_state.start = 0 };
				for (unsigned int j = 0; j < states.size(); j++) {
					if (states[j].state == temp_state.state) {
						temp_state.accept = states[j].accept;
						temp_state.start = states[j].start;
						break;
					}
				}
				queue.push_back(make_pair(temp_state, new_value));
			}
		}
		
		cout << new_value << endl;
	}

	cout << argc << endl;

	return 0;
}
