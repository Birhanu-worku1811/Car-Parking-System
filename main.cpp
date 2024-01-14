#include "iostream"
#include <fstream>
#include <cstdlib>
#include <iomanip> // for std::setw

using namespace std;


bool spotExists(const string& spot_id) {
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
    cout<<"List of all parking spots:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status" << "|" << endl;

    printLine();

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':')) {
            cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15) << "| " + car_type << setw(15) << "| " + status << "|" << endl;
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
    cout<<"List of Available spots:\n";
    cout << setw(10) << "| Floor" << setw(15) << "| Spot ID" << setw(15) << "| Car Type" << setw(15) << "| Status" << "|" << endl;

    printLine();

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string floor, spot_id, car_type, status;

        if (getline(iss, floor, ':') && getline(iss, spot_id, ':') &&
            getline(iss, car_type, ':') && getline(iss, status, ':') && status == "available") {
            cout << setw(10) << "| " + floor << setw(15) << "| " + spot_id << setw(15) << "| " + car_type << setw(15) << "| " + status << "|" << endl;
        }
    }

    printLine();
    cout << endl;

    file.close();
}

void reservedSpots() {

}

void searchSpots() {

}

void deleteSpots() {

}

void reserveSpots() {

}

void releaseSpots() {

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
                searchSpots();
                break;
            case '4':
                findCar();
                break;
            case '5':
                reserveSpots();
                break;
            case '6':
                releaseSpots();
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
    string floor, spot_id, car_type;
    cout << "\nWelcome Admin!, What do you want to do today?\n";
    while (choice != '0') {
        cout << "\n1. Add Parking Spots\n2. View All Spots\n3."
                " View Free Spots\n4. View Reserved Spots\n5. Search Spots\n6."
                " Delete spots\n7. Reserve Spots\n8. Release Spots\n9. logout\n0. exit\n\nEnter your choice:";
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
                searchSpots();
                break;
            case '6':
                deleteSpots();
                break;
            case '7':
                reserveSpots();
                break;
            case '8':
                releaseSpots();
                break;
            case '9':
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
        cout << "Invalid Credentials, Please try again!";
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
                }
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