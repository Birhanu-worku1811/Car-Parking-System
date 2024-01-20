#include "iostream"
#include <fstream>
#include <cstdlib>
#include <iomanip> // for std::setw
#include <ctime>   // for time()
#include <utility>

using namespace std;


struct Users{
    string username,phoneNumber,password;
    Users *next;
};

struct SpotNode {
    string floor, spotId, carType, status;
    SpotNode* left;
    SpotNode* right;

    SpotNode(string floor, string spotId, string carType, string status)
            : floor(std::move(floor)), spotId(std::move(spotId)), carType(std::move(carType)), status(std::move(status)), left(nullptr), right(nullptr) {}
};

struct ParkedCarNode {
    string plate, spotId, owner, password;
    ParkedCarNode* prev;
    ParkedCarNode* next;

    ParkedCarNode(string plate, string spotId, string owner, string password)
            : plate(std::move(plate)), spotId(std::move(spotId)), owner(std::move(owner)), password(std::move(password)), prev(nullptr), next(nullptr) {}
};

SpotNode* root = nullptr;
ParkedCarNode* parkedCarsHead = nullptr;

// resetting spotNode to avoid memory leakage
void resetSpotNodes(SpotNode* node) {
    if (node != nullptr) {
        resetSpotNodes(node->left);
        resetSpotNodes(node->right);
        delete node;
    }
}

// Resetting parkedCars doubly linked list to avoid memory leakage
void resetParkedCarsList() {
    ParkedCarNode* current = parkedCarsHead;
    while (current != nullptr) {
        ParkedCarNode* next = current->next;
        delete current;
        current = next;
    }
    parkedCarsHead = nullptr;
}

SpotNode* loadSpots() {
    // releasing memory of previous spotNode
    resetSpotNodes(root);
    ifstream file("parking_spots.txt");
    string line;


    // Function to insert a spot into the BST
    auto insertSpot = [&](const string& floor, const string& spotId, const string& carType, const string& status) {
        if (root == nullptr) {
            root = new SpotNode(floor, spotId, carType, status);
            return;
        }

        SpotNode* currentNode = root;
        while (true) {
            if (spotId < currentNode->spotId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new SpotNode(floor, spotId, carType, status);
                    return;
                } else {
                    currentNode = currentNode->left;
                }
            } else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new SpotNode(floor, spotId, carType, status);
                    return;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }
    };

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spotId, carType, status;

        if (getline(iss, floor, ':') && getline(iss, spotId, ':') &&
            getline(iss, carType, ':') && getline(iss, status, ':')) {
            insertSpot(floor, spotId, carType, status);
        }
    }

    file.close();
    return root;
}

ParkedCarNode* loadParkedCars() {
    resetParkedCarsList();
    ifstream file("parked_cars.txt");
    string line;

    ParkedCarNode* head = nullptr;
    ParkedCarNode* current = nullptr;

    while (getline(file, line)) {
        istringstream iss(line);
        string plate, spotId, owner, password;

        if (getline(iss, plate, ':') && getline(iss, spotId, ':') &&
            getline(iss, owner, ':') && getline(iss, password, ':')) {
            ParkedCarNode* newNode = new ParkedCarNode(plate, spotId, owner, password);

            if (head == nullptr) {
                head = newNode;
                current = newNode;
            } else {
                current->next = newNode;
                newNode->prev = current;
                current = newNode;
            }
        }
    }

    file.close();
    return head;
}

bool spotExists(const string &spot_id) {
    ifstream file("parking_spots.txt");
    string line;

    while (getline(file, line)) {
        int pos1 = line.find(':');
        if (pos1 != string::npos) {
            int pos2 = line.find(':', pos1 + 1);
            if (pos2 != string::npos) {
                string storedSpotId = line.substr(pos1 + 1, pos2 - pos1 - 1);

                if (storedSpotId == spot_id) {
                    file.close();
                    return true; // Spot with the given spot_id already exists
                }
            }
        }
    }

    file.close();
    return false; // Spot with the given spot_id does not exist
}

void addSpot(const string& floor, const string& spot_id, const string& car_type) {
    if (spotExists(spot_id)) {
        cout << "Spot with ID " << spot_id << " already exists." << endl;
        return;
    }

    ofstream file("parking_spots.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    file << floor << ':' << spot_id << ':' << car_type << ":available" << endl;
    file.close();
    cout << "Spot added Successfully!" << endl;
}

void printLine() {
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
}
void inOrderTraversal(SpotNode* node, const string& spotType) {
    if (node != nullptr) {
        inOrderTraversal(node->left, spotType);

        if (spotType == "free" && node->status == "available") {
            cout << setw(10) << "| " + node->floor << setw(15) << "| " + node->spotId << setw(15)
                 << "| " + node->carType << setw(15) << "| " + node->status << "|" << endl;
        } else if (spotType == "reserved" && node->status == "reserved") {
            cout << setw(10) << "| " + node->floor << setw(15) << "| " + node->spotId << setw(15)
                 << "| " + node->carType << setw(15) << "| " + node->status << "|" << endl;
        } else if (spotType == "all") {
            cout << setw(10) << "| " + node->floor << setw(15) << "| " + node->spotId << setw(15)
                 << "| " + node->carType << setw(15) << "| " + node->status << "|" << endl;
        }

        inOrderTraversal(node->right, spotType);
    }
}

void listSpots(SpotNode* spot) {
    if (spot == nullptr) {
        cout << "No parking spots available.\n";
        return;
    }
    cout << setiosflags(ios::left);
    cout << "List of all parking spots:\n";
    printLine();
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;
    printLine();

    // Call the inOrderTraversal function for all spots
    inOrderTraversal(spot, "all");

    printLine();
    cout << endl;
}

void freeSpots(SpotNode* spot) {
    if (spot == nullptr) {
        cout << "No parking spots available.\n";
        return;
    }
    cout << setiosflags(ios::left);
    cout << "List of Available spots:\n";
    printLine();
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;
    printLine();

    // Call the inOrderTraversal function for free spots
    inOrderTraversal(spot, "free");

    printLine();
    cout << endl;
}

void reservedSpots(SpotNode* spot) {
    if (spot == nullptr) {
        cout << "No reserved parking spots.\n";
        return;
    }
    cout << setiosflags(ios::left);
    cout << "List of Reserved spots:\n";
    printLine();
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;
    printLine();

    // Call the inOrderTraversal function for reserved spots
    inOrderTraversal(spot, "reserved");

    printLine();
    cout << endl;
}

//for searching spots by floor and car_type
void preOrderTraversal(SpotNode* spot, const string& search_by, const string& key) {
    if (spot != nullptr) {
        if ((search_by == "floor" && spot->floor == key) ||
            (search_by == "car_type" && spot->carType == key)) {
            cout << setiosflags(ios::left);
            cout << setw(10) << "| " + spot->floor << setw(15) << "| " + spot->spotId << setw(15)
                 << "| " + spot->carType << setw(15) << "| " + spot->status << "|" << endl;
        }

        preOrderTraversal(spot->left, search_by, key);
        preOrderTraversal(spot->right, search_by, key);
    }
}

void searchSpots(const string& search_by, const string& key, SpotNode* spot) {
    if (spot == nullptr) {
        cout << "No parking spots available.\n";
        return;
    }

    if (search_by == "spot_id") {
        // Binary search in the SpotNode for spot_id
        SpotNode* currentNode = spot;
        while (currentNode != nullptr) {
            if (key == currentNode->spotId) {
                // Found the spot, print information
                cout << setiosflags(ios::left);
                cout << "Search results:\n";
                printLine();
                cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
                     << "|" << endl;
                printLine();
                cout << setw(10) << "| " + currentNode->floor << setw(15) << "| " + currentNode->spotId << setw(15)
                     << "| " + currentNode->carType << setw(15) << "| " + currentNode->status << "|" << endl;
                printLine();
                return;
            } else if (key < currentNode->spotId) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }
        // If the loop completes, the spot with the given spot_id is not found
        cout << "Spot with ID " << key << " not found." << endl;
    } else {
        // Linear search in the SpotNode for floor or car_type
        printLine();
        cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
             << "|" << endl;
        printLine();
        preOrderTraversal(spot, search_by, key);
        printLine();
    }
}


void deleteSpots(const string& spot_id) {
    ifstream file("parking_spots.txt");
    ofstream tempFile("temp.txt");

    string line;

    while (getline(file, line)) {
        int pos1 = line.find(':');
        if (pos1 != string::npos) {
            int pos2 = line.find(':', pos1 + 1);
            if (pos2 != string::npos) {
                string storedSpotId = line.substr(pos1 + 1, pos2 - pos1 - 1);

                if (storedSpotId != spot_id) {
                    tempFile << line << endl;
                }
            }
        }
    }

    file.close();
    tempFile.close();

    // Replace the old file with the new one
    remove("parking_spots.txt");
    rename("temp.txt", "parking_spots.txt");

    cout << "Spot with ID " << spot_id << " deleted Successfully!" << endl;
}

// Function to generate a random 6-digit password
string generatePassword() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation
    int password = rand() % 900000 + 100000; // Generate a random 6-digit number
    return to_string(password);
}

void reserveSpots(const string& car_type, const string& spot_id) {
    // Check if the spot exists and is available
    ifstream file("parking_spots.txt");
    string line;

    while (getline(file, line)) {
        int pos1 = line.find(':');
        if (pos1 != string::npos) {
            int pos2 = line.find(':', pos1 + 1);
            if (pos2 != string::npos) {
                string storedSpotId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string storedCarType = line.substr(pos2 + 1, line.find(':', pos2 + 1) - pos2 - 1);
                string status = line.substr(line.rfind(':') + 1);

                if (storedSpotId == spot_id && storedCarType == car_type) {
                    if (status == "available") {
                        // Generate a random password
                        string password = generatePassword();

                        // Store car information along with the password in parked_cars.txt
                        ofstream parkedFile("parked_cars.txt", ios::app);
                        if (!parkedFile.is_open()) {
                            cout << "Error opening file for writing!" << endl;
                            return;
                        }

                        // Get car plate and owner information
                        string car_plate, owner;
                        cout << "Enter car plate: ";
                        cin >> car_plate;
                        cout << "Enter owner's name: ";
                        cin >> owner;

                        // Store car information and password in parked_cars.txt
                        parkedFile << car_plate << ':' << spot_id << ':' << owner << ':' << password << endl;
                        parkedFile.close();

                        // Update the status of the spot in parking_spots.txt to reserved
                        ifstream inputFile("parking_spots.txt");
                        ofstream tempFile("temp.txt");

                        while (getline(inputFile, line)) {
                            int pos1 = line.find(':');
                            if (pos1 != string::npos) {
                                int pos2 = line.find(':', pos1 + 1);
                                if (pos2 != string::npos) {
                                    string storedSpotId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                                    if (storedSpotId == spot_id) {
                                        tempFile << line.substr(0, line.rfind(':')) << ":reserved" << endl;
                                    } else {
                                        tempFile << line << endl;
                                    }
                                }
                            }
                        }

                        inputFile.close();
                        tempFile.close();

                        // Replace the old file with the new one
                        remove("parking_spots.txt");
                        rename("temp.txt", "parking_spots.txt");

                        cout << "Spot reserved successfully! Password: " << password << endl;
                        return;
                    } else {
                        cout << "Spot with ID " << spot_id << " is already reserved." << endl;
                        return;
                    }
                }
            }
        }
    }

    file.close();
    cout << "Spot with ID " << spot_id << " not found or not available." << endl;
}

void releaseSpot(const string& spot_id) {
    ifstream file("parking_spots.txt");
    string line;

    while (getline(file, line)) {
        int pos1 = line.find(':');
        if (pos1 != string::npos) {
            int pos2 = line.find(':', pos1 + 1);
            if (pos2 != string::npos) {
                string storedSpotId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string status = line.substr(line.rfind(':') + 1);

                if (storedSpotId == spot_id) {
                    if (status == "available") {
                        cout << "Spot with ID " << spot_id << " was not reserved." << endl;
                        return;
                    } else {
                        // Spot is reserved, find the corresponding entry in parked_cars.txt
                        ifstream parkedFile("parked_cars.txt");
                        string parkedLine;

                        while (getline(parkedFile, parkedLine)) {
                            int pos1_parked = parkedLine.find(':');
                            if (pos1_parked != string::npos) {
                                int pos2_parked = parkedLine.find(':', pos1_parked + 1);
                                if (pos2_parked != string::npos) {
                                    string storedSpotId_parked = parkedLine.substr(pos1_parked + 1, pos2_parked - pos1_parked - 1);
                                    string owner = parkedLine.substr(pos2_parked + 1, parkedLine.rfind(':') - pos2_parked - 1);
                                    string password = parkedLine.substr(parkedLine.rfind(':') + 1);

                                    if (storedSpotId_parked == spot_id) {
                                        cout << "This spot was reserved by " << owner << "." << endl;

                                        // Ask for password to release the spot
                                        string enteredPassword;
                                        cout << "Enter the password to release the spot: ";
                                        cin >> enteredPassword;

                                        if (enteredPassword == password) {
                                            // Password matches, delete the car from parked_cars.txt
                                            ifstream inputFile("parked_cars.txt");
                                            ofstream tempFile("temp.txt");

                                            while (getline(inputFile, parkedLine)) {
                                                if (parkedLine != parkedLine) {
                                                    tempFile << parkedLine << endl;
                                                }
                                            }

                                            inputFile.close();
                                            tempFile.close();

                                            // Replace the old file with the new one
                                            remove("parked_cars.txt");
                                            rename("temp.txt", "parked_cars.txt");

                                            // Update the status of the spot in parking_spots.txt to available
                                            ifstream inputFileSpots("parking_spots.txt");
                                            ofstream tempFileSpots("temp.txt");

                                            while (getline(inputFileSpots, line)) {
                                                int pos1_spots = line.find(':');
                                                if (pos1_spots != string::npos) {
                                                    int pos2_spots = line.find(':', pos1_spots + 1);
                                                    if (pos2_spots != string::npos) {
                                                        string storedSpotId_spots = line.substr(pos1_spots + 1, pos2_spots - pos1_spots - 1);
                                                        if (storedSpotId_spots == spot_id) {
                                                            tempFileSpots << line.substr(0, line.rfind(':')) << ":available" << endl;
                                                        } else {
                                                            tempFileSpots << line << endl;
                                                        }
                                                    }
                                                }
                                            }

                                            inputFileSpots.close();
                                            tempFileSpots.close();

                                            // Replace the old file with the new one
                                            remove("parking_spots.txt");
                                            rename("temp.txt", "parking_spots.txt");

                                            cout << "Spot released successfully!" << endl;
                                            return;
                                        } else {
                                            cout << "Incorrect password! Spot release failed." << endl;
                                            return;
                                        }
                                    }
                                }
                            }
                        }

                        parkedFile.close();
                        cout << "Spot with ID " << spot_id << " not found in parked_cars.txt." << endl;
                        return;
                    }
                }
            }
        }
    }

    file.close();
    cout << "Spot with ID " << spot_id << " not found." << endl;
}

void findCar(ParkedCarNode* head, const string& plate) {
    ParkedCarNode* current = head;

    while (current != nullptr) {
        if (current->plate == plate) {
            // Ask for user password for verification
            string enteredPassword;
            cout << "Enter your password to verify your identity: ";
            cin >> enteredPassword;

            // Verify the password
            if (enteredPassword != current->password) {
                cout << "Incorrect password! Access denied.\n";
                return;
            }

            cout << "Car with plate " << plate << " found!\n";
            cout << "Owner: " << current->owner << "\n";

            // Now, find the spot information in parking_spots.txt
            ifstream spotsFile("parking_spots.txt");
            string spotLine;

            while (getline(spotsFile, spotLine)) {
                int pos1_spots = spotLine.find(':');
                if (pos1_spots != string::npos) {
                    int pos2_spots = spotLine.find(':', pos1_spots + 1);
                    if (pos2_spots != string::npos) {
                        string floor = spotLine.substr(0, spotLine.find(':'));
                        string storedSpotId_spots = spotLine.substr(pos1_spots + 1, pos2_spots - pos1_spots - 1);
                        string carType = spotLine.substr(pos2_spots + 1, spotLine.rfind(':') - pos2_spots - 1);
                        string status = spotLine.substr(spotLine.rfind(':') + 1);

                        if (storedSpotId_spots == current->spotId) {
                            cout << "Floor: " << floor << endl;
                            cout << "Spot ID: " << storedSpotId_spots << "\n";
                            cout << "Car Type: " << carType << "\n";
                            spotsFile.close();
                            return;
                        }
                    }
                }
            }

            spotsFile.close();
            cout << "Spot information not found for car with plate " << plate << "\n";
            return;
        }

        current = current->next;
    }

    cout << "Car with plate " << plate << " not found!\n";
}

void userMenu(const string &username) {
    char choice;
    string search_by, plate, car_type, spot_id;
    cout << "\nWelcome " << username << "!\n";
    while (choice != '8') {
        cout << "\n1.List all spots\n2. List Free Spots\n3."
                " Search Spots\n4. Where is My car\n5."
                " Reserve spot\n6. Release spot\n0."
                " Exit\n\nEnter your choice:";
        cin >> choice;
        switch (choice) {
            case '1':
                listSpots(loadSpots());
                break;
            case '2':
                freeSpots(loadSpots());
                break;
            case '3':
                do {
                    cout<<"Search by(floor, car_type, spot_id)?\n";
                    cin>>search_by;
                    if (search_by != "floor" && search_by != "car_type" && search_by != "spot_id")
                        cout<<"Invalid search key! please type correctly";
                } while (search_by != "floor" && search_by != "car_type" && search_by != "spot_id");
                if (search_by == "floor") {
                    string flr;
                    cout << "Enter the floor number:";
                    cin>>flr;
                    searchSpots("floor", flr, loadSpots());
                } else if (search_by == "car_type"){
                    string crt;
                    do {
                        cout<<"Choose one(auto, bus, truck):";
                        cin>>crt;
                        if (crt !="auto" && crt != "bus" && crt != "truck")
                            cout<<"Invalid car type!\n";
                    } while (crt !="auto" && crt != "bus" && crt != "truck");
                    searchSpots("car_type", crt, loadSpots());
                } else{
                    string id;
                    cout<<"Enter the spot id:";
                    cin>>id;
                    searchSpots("spot_id", id, loadSpots());
                }
                break;
            case '4':
                cout<<"\nEnter the plate of the car:";
                cin>>plate;
                findCar(loadParkedCars(), plate);
                break;
            case '5':
                cout<<endl;
                listSpots(loadSpots());
                cout<<"\nWhat type of car do you want to park(auto, bus, truck):";
                cin>>car_type;
                cout<<"\nWhere do you want the car to park(Spot ID):";
                cin>>spot_id;
                reserveSpots(car_type, spot_id);
                loadSpots();
                break;
            case '6':
                cout<<"Enter the spot ID you want to release:";
                cin>>spot_id;
                releaseSpot(spot_id);
                loadSpots();
                break;
            case '0':
                exit(0);
        }
    }
}


// to load users from file and store in linked list
Users* loadUsers() {
    ifstream file("users.txt");
    string line;

    Users *head = nullptr;
    Users *current = nullptr;

    while (getline(file, line)) {
        istringstream iss(line);
        string username, phoneNumber, password;

        if (getline(iss, username, ':') && getline(iss, phoneNumber, ':') && getline(iss, password, ':')) {
            Users *newUser = new Users{username, phoneNumber, password, nullptr};

            if (head == nullptr) {
                head = newUser;
                current = newUser;
            } else {
                current->next = newUser;
                current = newUser;
            }
        }
    }

    file.close();
    return head;
}

void printUsers(Users *head) {
    cout << "\nList of Users:\n";
    cout << setw(25) << left << "| Username" << setw(25) << left << "| Phone Number" << setw(25) << left << "| Password" << "|" << endl;
    printLine();
    //for skipping the table head
    int a = 0;
    while (head != nullptr) {
        if (a == 0) {
            a++;
            head = head->next;
            continue;
        }
        cout << setw(25) << left << "| " + head->username << setw(25) << left << "| " + head->phoneNumber << setw(25) << left << "| " + head->password << "|" << endl;
        head = head->next;
    }
    printLine();
}

void searchUser(){
    Users *head = loadUsers();
    string username;
    cout<<"Enter the username you want to search:";
    cin>>username;
    while (head != nullptr) {
        if (head->username == username) {
            cout << "User found!\n";
            cout << "Username: " << head->username << "\n";
            cout << "Phone Number: " << head->phoneNumber << "\n";
            cout << "Password: " << head->password << "\n";
            return;
        }
        head = head->next;
    }
    cout << "User not found!\n";
}

void adminMenu() {
    char choice;
    string search_by;
    string floor, spot_id, car_type;
    cout << "\nWelcome Admin!, What do you want to do today?\n";
    while (choice != '0') {
        cout << "\n1. Add Parking Spots\n2. View All Spots\n3."
                " View Free Spots\n4. View Reserved Spots\n5. Search Spots\n6."
                " Delete spots\n7. Reserve Spots\n8. Release Spots\n9. View all users\nA. Search user\n0. exit\n\nEnter your choice:";
        cin >> choice;
        switch (choice) {
            case '1':
                cout << "Enter the floor number: ";
                cin >> floor;
                cout << "Enter ID of the parking spot:";
                cin >> spot_id;
                do {
                    cout << "Choose vehicle type(auto, bus, truck):";
                    cin >> car_type;
                    if (car_type != "auto" && car_type != "bus" && car_type != "truck") {
                        cout << "Invalid choice, Try again!\n";
                    }
                } while (car_type != "auto" && car_type != "bus" && car_type != "truck");
                addSpot(floor, spot_id, car_type);
                break;
            case '2':
                listSpots(loadSpots());
                break;
            case '3':
                freeSpots(loadSpots());
                break;
            case '4':
                reservedSpots(loadSpots());
                break;
            case '5':
                do {
                    cout<<"Search by(floor, car_type, spot_id)?\n";
                    cin>>search_by;
                    if (search_by != "floor" && search_by != "car_type" && search_by != "spot_id")
                        cout<<"Invalid search key! please type correctly";
                } while (search_by != "floor" && search_by != "car_type" && search_by != "spot_id");
                if (search_by == "floor") {
                    string flr;
                    cout << "Enter the floor number:";
                    cin>>flr;
                    searchSpots("floor", flr, loadSpots());
                } else if (search_by == "car_type"){
                    string crt;
                    do {
                        cout<<"Choose one(auto, bus, truck):";
                        cin>>crt;
                        if (crt !="auto" && crt != "bus" && crt != "truck")
                            cout<<"Invalid car type!\n";
                    } while (crt !="auto" && crt != "bus" && crt != "truck");
                    searchSpots("car_type", crt, loadSpots());
                } else{
                    string id;
                    cout<<"Enter the spot id:";
                    cin>>id;
                    searchSpots("spot_id", id, loadSpots());
                }
                break;
            case '6':
                cout<<endl;
                listSpots(loadSpots());
                cout<<"\nEnter the spot id you want to delete:\n";
                cin>>spot_id;
                deleteSpots(spot_id);
                loadSpots();
                break;
            case '7':
                cout<<endl;
                listSpots(loadSpots());
                cout<<"\nWhat type of car do you want to park(auto, bus, truck):";
                cin>>car_type;
                cout<<"\nWhere do you want the car to park(Spot ID):";
                cin>>spot_id;
                reserveSpots(car_type, spot_id);
                loadSpots();
                break;
            case '8':
                cout<<"Enter the spot ID you want to release:";
                cin>>spot_id;
                releaseSpot(spot_id);
                loadSpots();
                break;
            case '9':
                printUsers(loadUsers());
                break;
            case 'A':
                searchUser();
                break;
            case '0':
                exit(0);
            default:
                cout << "Invalid choice, try again";
        }
    }
}

void userLogin(const string &username, const string &password) {
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        int pos1 = line.find(':');
        if (pos1 != string::npos) {
            size_t pos2 = line.find(':', pos1 + 1);
            if (pos2 != string::npos) {
                string storedUsername = line.substr(0, pos1);
                string storedPhoneNumber = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string storedPassword = line.substr(pos2 + 1);

                if (storedUsername == username && storedPassword == password) {
                    file.close();
                    cout << "Login successful!" << endl;
                    userMenu(username);
                }
            }
        }
    }

    file.close();
    cout << "Invalid Credentials, Try again!" << endl;
}

void adminLogin(const string password) {
    string adminPassword = "admin";
    if (password == adminPassword) {
        cout << "Login Success!";
        adminMenu();
    } else {
        cout << "Invalid Password, Please try again!";
    }
}

void registerUser(string username, string phone_number, string password) {

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    file << username << ':' << phone_number << ':' << password << endl;
    file.close();
    cout << "Registration Success!" << endl;
}

int main() {
    char choice;
    string username, phone_number, password, password_confirmed;
    cout << "\n\n\t\t\t\t\t\t====WELCOME======\n";
    while (choice != '3') {
        cout << "\n1. Login\n2. Register\n3. Exit\n" << endl;
        cout << "\n\nEnter your Choice:";
        cin >> choice;

        switch (choice) {
            case '1':
                char ch;
                do {
                    cout << "\n1. Admin\n2. User\n3. back\n";
                    cout << "Enter Choice:";
                    cin >> ch;
                    switch (ch) {
                        case '1':
                            cout << "Enter Password:";
                            cin >> password;
                            adminLogin(password);
                            break;
                        case '2':
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password";
                            cin >> password;
                            userLogin(username, password);
                            break;
                        case '3':
                            break;
                        default:
                            cout << "Invalid Choice! Enter Again";
                            break;
                    }
                } while (ch != '3');
                break;
            case '2':
                cout << "Enter your name: ";
                cin >> username;
                cout << "Enter your phone number";
                cin >> phone_number;
                do {
                    cout << "Enter Password: ";
                    cin >> password;
                    cout << "Confirm your password: ";
                    cin >> password_confirmed;
                    if (password != password_confirmed) {
                        cout << "Passwords do not match. Please try again.\n";
                    }
                } while (password != password_confirmed);
                registerUser(username, phone_number, password);
                break;
            case '3':
                exit(0);
            default:
                cout << "Invalid Choice! Enter Again";
        }
    }
    return 0;
}