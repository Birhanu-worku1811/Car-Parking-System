#include "iostream"
#include <fstream>
#include <cstdlib>
#include <iomanip> // for std::setw
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

using namespace std;


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

void addSpot(string floor, string spot_id, string car_type) {
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

void listSpots() {
    ifstream file("parking_spots.txt");
    string line;

    cout << setiosflags(ios::left);
    cout << "List of all parking spots:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;

    printLine();

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':')) {
            cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15) << "| " + car_type << setw(15)
                 << "| " + status << "|" << endl;
        }
    }

    printLine();
    cout << endl;

    file.close();
}

void freeSpots() {
    ifstream file("parking_spots.txt");
    string line;

    cout << setiosflags(ios::left);
    cout << "List of Available spots:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;

    printLine();

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':') && status == "available") {
            cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15) << "| " + car_type << setw(15)
                 << "| " + status << "|" << endl;
        }
    }

    printLine();
    cout << endl;

    file.close();
}

void reservedSpots() {
    ifstream file("parking_spots.txt");
    string line;

    cout << setiosflags(ios::left);
    cout << "List of Available spots:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;

    printLine();

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':') && status == "reserved") {
            cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15) << "| " + car_type << setw(15)
                 << "| " + status << "|" << endl;
        }
    }

    printLine();
    cout << endl;

    file.close();
}

void searchSpots(const string& search_by, const string& key) {
    ifstream file("parking_spots.txt");
    string line;

    cout << setiosflags(ios::left);
    cout << "Search results:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status"
         << "|" << endl;

    printLine(); // Assume you have a function to print a line

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':')) {

            if (search_by == "floor" && floor == key) {
                cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15)
                     << "| " + car_type << setw(15) << "| " + status << "|" << endl;
            } else if (search_by == "car_type" && car_type == key) {
                cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15)
                     << "| " + car_type << setw(15) << "| " + status << "|" << endl;
            } else if (search_by == "spot_id" && spot_id == key) {
                cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15)
                     << "| " + car_type << setw(15) << "| " + status << "|" << endl;
            }
        }
    }

    printLine();
    cout << endl;

    file.close();
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

                        cout << "Spot reserved successfully! Password(tell this to the car owner): " << password << endl;
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


void findCar() {

}

void userMenu(const string &username) {
    char choice;
    cout << "\nWelcome " << username << "!\n";
    while (choice != '8') {
        cout << "\n1.List all spots\n2. List Free Spots\n3."
                " Search Spots\n4. Where is My car\n5."
                " Reserve spot\n6. Release spot\n7. Logout\n8."
                " Exit\n\nEnter your choice:";
        cin >> choice;
        switch (choice) {
            case '1':
                listSpots();
                break;
            case '2':
                freeSpots();
                break;
            case '3':
//                searchSpots();
                break;
            case '4':
                findCar();
                break;
            case '5':
//                reserveSpots();
                break;
            case '6':
//                releaseSpots();
                break;
            case '7':
                break;
            case '8':
                exit(0);
        }
    }
}

void adminMenu() {
    char choice;
    string search_by;
    string floor, spot_id, car_type;
    cout << "\nWelcome Admin!, What do you want to do today?\n";
    while (choice != '0') {
        cout << "\n1. Add Parking Spots\n2. View All Spots\n3."
                " View Free Spots\n4. View Reserved Spots\n5. Search Spots\n6."
                " Delete spots\n7. Reserve Spots\n8. Release Spots\n0. exit\n\nEnter your choice:";
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
                listSpots();
                break;
            case '3':
                freeSpots();
                break;
            case '4':
                reservedSpots();
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
                    searchSpots("floor", flr);
                } else if (search_by == "car_type"){
                    string crt;
                    do {
                        cout<<"Choose one(auto, bus, truck):";
                        cin>>crt;
                        if (crt !="auto" && crt != "bus" && crt != "truck")
                            cout<<"Invalid car type!\n";
                    } while (crt !="auto" && crt != "bus" && crt != "truck");
                    searchSpots("car_type", crt);
                } else{
                    string id;
                    cout<<"Enter the spot id:";
                    cin>>id;
                    searchSpots("spot_id", id);
                }
                break;
            case '6':
                cout<<endl;
                listSpots();
                cout<<"\nEnter the spot id you want to delete:\n";
                cin>>spot_id;
                deleteSpots(spot_id);
                break;
            case '7':
                cout<<endl;
                listSpots();
                cout<<"\nWhat type of car do you want to park(auto, bus, truck):";
                cin>>car_type;
                cout<<"\nWhere do you want the car to park(Spot ID):";
                cin>>spot_id;
                reserveSpots(car_type, spot_id);
                break;
            case '8':
                cout<<"Enter the spot ID you want to release:";
                cin>>spot_id;
                releaseSpot(spot_id);
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

void adminLogin(string password) {
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