/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Raqeebullah Hasan and Andrew Hing
 * Date        : 10-21-22
 * Description : Solve water jug puzzle
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
  int a, b, c;
  string directions;
  State *parent;

  State(int _a, int _b, int _c, string _directions)
      : a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} {}

  // String representation of state in tuple form.
  string to_string() {
    ostringstream oss;
    oss << "(" << a << ", " << b << ", " << c << ")";
    return oss.str();
  }
};

string display(stack<string>&result){
	ostringstream oss;
	// print the result
  while (not result.empty()) {
    oss << result.top(); // print the steps!!!
    result.pop();
		if(not result.empty())
			oss << "\n";
  }
  return oss.str();
}

string bfs(int capA, int capB, int capC, int goalA, int goalB, int goalC) {
  vector<vector<State *>> matrix(capA + 1, vector<State *>(capB + 1));

  for (auto &rows : matrix) {
    for (auto &columns : rows) {
      columns = nullptr; // Initiliazing the array
    }
  }

  State *initial = new State(0, 0, capC, "Initial");
  State *current = new State(0, 0, capC, "Initial");

  queue<State *> stateQueue;
  stateQueue.push(current);

  while (not stateQueue.empty()) {
    current = stateQueue.front();
    stateQueue.pop();

		// Check if the goal is achieved: current == goal
    if (current->a == goalA && current->b == goalB &&
        current->c == goalC) { // Checkboard Flag

      stack<string> result;    // to store the pour result
      State* latest = current;

      while (current->directions != "Initial") {
        result.push(current->directions); // push the directions until initial
        current = current->parent;        // climb back
      }

      current = latest;
      delete current; // delete the current

      result.push("Initial state. " + initial->to_string());
      delete initial; // delete the intial

      // deleting the queue
      while (not stateQueue.empty()) {
        delete stateQueue.front();
        stateQueue.pop();
      }

      for (auto &rows : matrix)
          for (auto &columns : rows)
            delete columns; // deleting the array

      return display(result);
    } // Checkboard Flag

    if(matrix[current->a][current->b] != nullptr){
    	delete current;
		continue;
	}else{
		matrix[current->a][current->b] = current;
	}

    // Six Ways to pour water--------------------

    // A into B starts
    if ((current->a != 0) && (current->b != capB)) {
      int quantityB = capB - current->b; // remaining quantity of jug B
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityB >= current->a) { // if the a can be poured into b
        newState->b += newState->a;
        newState->a -= 0; // empty the jug A

        if (current->a == 1)
          direction += "Pour " + to_string(current->a) +
                       " gallon from A to B. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->a) +
                       " gallons from A to B. " + newState->to_string();
      } else { // Only pour the amount from A to fill the jug B
        newState->b += quantityB;
        newState->a -= quantityB;

        if (quantityB == 1)
          direction += "Pour " + to_string(quantityB) +
                       " gallon from A to B. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityB) +
                       " gallons from A to B. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // A into B ends

    // A into C starts
    if ((current->a != 0) && (current->c != capC)) {
      int quantityC = capC - current->c; // remaining quantity of jug C
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityC >= current->a) { // if the a can be poured into c
        newState->c += newState->a;
        newState->a = 0; // empty the jug A

        if (current->a == 1)
          direction += "Pour " + to_string(current->a) +
                       " gallon from A to C. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->a) +
                       " gallons from A to C. " + newState->to_string();
      } else { // Only pour the amount from A to fill the jug C
        newState->c += quantityC;
        newState->a -= quantityC;

        if (quantityC == 1)
          direction += "Pour " + to_string(quantityC) +
                       " gallon from A to C. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityC) +
                       " gallons from A to C. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // A into C ends

    // B into A starts
    if ((current->b != 0) && (current->a != capA)) {
      int quantityA = capA - current->a; // remaining quantity of jug B
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityA >= current->b) { // if the B jug can be poured into A
        newState->a += newState->b;
        newState->b = 0; // empty the jug B

        if (current->b == 1)
          direction += "Pour " + to_string(current->b) +
                       " gallon from B to A. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->b) +
                       " gallons from B to A. " + newState->to_string();
      } else { // Only pour the amount from B to fill the jug A
        newState->a += quantityA;
        newState->b -= quantityA;

        if (quantityA == 1)
          direction += "Pour " + to_string(quantityA) +
                       " gallon from B to A. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityA) +
                       " gallons from B to A. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // B into A ends

    // B into C starts
    if ((current->b != 0) && (current->c != capC)) {
      int quantityC = capC - current->c; // remaining quantity of jug C
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityC >= current->b) { // if the C jug can be poured into A
        newState->c += newState->b;
        newState->b = 0; // empty the jug B

        if (current->b == 1)
          direction += "Pour " + to_string(current->b) +
                       " gallon from B to C. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->b) +
                       " gallons from B to C. " + newState->to_string();
      } else { // Only pour the amount from B to fill the jug C
        newState->c += quantityC;
        newState->b -= quantityC;

        if (quantityC == 1)
          direction += "Pour " + to_string(quantityC) +
                       " gallon from B to C. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityC) +
                       " gallons from B to C. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // B into C ends

    // C into A starts
    if ((current->c != 0) && (current->a != capA)) {
      int quantityA = capA - current->a; // remaining quantity of jug A
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityA >= current->c) { // if the A jug can be poured into C
        newState->a += newState->c;
        newState->c = 0; // empty the jug C

        if (current->c == 1)
          direction += "Pour " + to_string(current->c) +
                       " gallon from C to A. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->c) +
                       " gallons from C to A. " + newState->to_string();
      } else { // Only pour the amount from C to fill the jug A
        newState->a += quantityA; // full
        newState->c -= quantityA;

        if (quantityA == 1)
          direction += "Pour " + to_string(quantityA) +
                       " gallon from C to A. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityA) +
                       " gallons from C to A. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // C into A ends

    // C into B starts
    if ((current->c != 0) && (current->b != capB)) {
      int quantityB = capB - current->b; // remaining quantity of jug A
      State *newState = new State(current->a, current->b, current->c, "None");
      string direction;
      newState->parent = current;

      if (quantityB >= current->c) { // if the B jug can be poured into C
        newState->b += newState->c;
        newState->c = 0; // empty the jug C

        if (current->b == 1)
          direction += "Pour " + to_string(current->b) +
                       " gallon from C to B. " + newState->to_string();
        else
          direction += "Pour " + to_string(current->b) +
                       " gallons from C to B. " + newState->to_string();
      } else { // Only pour the amount from C to fill the jug B
        newState->b += quantityB; // full
        newState->c -= quantityB;

        if (quantityB == 1)
          direction += "Pour " + to_string(quantityB) +
                       " gallon from C to B. " + newState->to_string();
        else
          direction += "Pour " + to_string(quantityB) +
                       " gallons from C to B. " + newState->to_string();
      }

      newState->directions = direction;

      // Check if newState is already in the matrix
      if (matrix[newState->a][newState->b] != nullptr)
        delete newState;
      else
        stateQueue.push(newState);

    } // C into B ends

		//debug
		//cout << stateQueue.front()->a << "---" << stateQueue.front()->b << "----" << stateQueue.front()->c << endl;
		//cout << "Goals:--" << goalA << "------" << goalB << "------" << goalC<< endl;
  }

  for (auto &rows : matrix)
    for (auto &columns : rows)
      delete columns; // deleting the array

  delete initial;

  return "No solution.";
}

int main(int argc, char *argv[]) {

  string inputs_letters[6] = {"A", "B", "C", "A", "B", "C"};
  string inputs_type[6] = {"capacity", "capacity", "capacity",
                           "goal",     "goal",     "goal"};
  istringstream iss;

  if (argc != 7) { // check for corrent amount of inputs
    cerr << "Usage: " << argv[0]
         << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
    return 1;
  }

  vector<int> list(6);
  for (int i = 1; i < 7; i++) {
    iss.str(argv[i]);
    if (!(iss >> list[i - 1]) || list[(i - 1) % 3] <= 0 ||
        list[i - 1] < 0) { // check to see if each input is an int or capacity
                           // is > 0 or goal is >= 0
      cerr << "Error: Invalid " << inputs_type[i - 1] << " '" << argv[i] << "' "
           << "for jug " << inputs_letters[i - 1] << "." << endl;
      return 1;
    }
    iss.clear();
  }
  if (list[3] > list[0]) { // check if goal is greater than capacity
    cerr << "Error: Goal cannot exceed capacity of jug " << inputs_letters[3]
         << "." << endl;
    return 1;
  }
  if (list[4] > list[1]) {
    cerr << "Error: Goal cannot exceed capacity of jug " << inputs_letters[4]
         << "." << endl;
    return 1;
  }
  if (list[5] > list[2]) {
    cerr << "Error: Goal cannot exceed capacity of jug " << inputs_letters[5]
         << "." << endl;
    return 1;
  }
  if (list[3] + list[4] + list[5] !=
      list[2]) { // check if total gallons in goal state is greater than the
                 // capactiy of C
    cerr << "Error: Total gallons in goal state must be equal to the capacity "
            "of jug C."
         << endl;
    return 1;
  }

  if (bfs(list[0], list[1], list[2], list[3], list[4], list[5]) ==
      "No solution.") {
    cout << "No solution." << endl;
    return 1;
  }
  cout << bfs(list[0], list[1], list[2], list[3], list[4], list[5]) << endl;
  return 0;
}
