#include <iostream>
#include <vector>
#include <string>
#include<fstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <sstream>
#include <iomanip>
using namespace std;

struct Parcel {
    int id;
    string sender;
    string receiver;
    string status;
    int deliveryTime;
    int distance;
    int weight;
    string location;
    bool assigned = false;
    int assignedStaffId = -1; // -1 means unassigned
};

struct Staff {
    int id;
    string name;
    string joiningDate;
    int totalAssigned = 0; // Tracks how many parcels are assigned
};


vector<Parcel> parcels;
vector<Staff> staffList;
int nextParcelId = 1;

// UI helpers
void printLine() {
    cout << string(70, '-') << endl;
}
string centerAlign(const string& text, int width = 80) {
    int pad = (width - text.length()) / 2;
    if (pad > 0) return string(pad, ' ') + text;
    return text;
}

string repeat(char ch, int count) {
    return string(count, ch);
}


void centerText(const string& text) {
    int width = 70;
    int pad = (width - text.length()) / 2;
    if (pad > 0) cout << string(pad, ' ');
    cout << text << endl;
}

void centerList(const vector<string>& items) {
    for (const string& item : items) centerText(item);
}
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

void saveStaffData() {
    ofstream fout("staff.txt");

    fout << centerAlign("Project Title: Parcel Delivery Optimizer") << "\n";
    fout << centerAlign("STAFF RECORDS") << "\n\n";

    // Column widths
    const int wID = 4, wName = 11, wDate = 16, wTotal = 16;
    string separator = "+" + repeat('-', wID + 2) + "+" + repeat('-', wName + 2) + "+" +
                       repeat('-', wDate + 2) + "+" + repeat('-', wTotal + 2) + "+";

    fout << separator << "\n";
    fout << "| " << setw(wID) << left << "ID"
         << " | " << setw(wName) << left << "NAME"
         << " | " << setw(wDate) << left << "JOINING DATE"
         << " | " << setw(wTotal) << left << "TOTAL ASSIGNED" << " |\n";
    fout << separator << "\n";

    for (const Staff& s : staffList) {
        fout << "| " << setw(wID) << left << s.id
             << " | " << setw(wName) << left << s.name
             << " | " << setw(wDate) << left << s.joiningDate
             << " | " << setw(wTotal) << left << s.totalAssigned << " |\n";
    }

    fout << separator << "\n";
    fout.close();
}


void loadStaffData() {
    ifstream fin("staff.txt");
    if (!fin.is_open()) return;

    staffList.clear();
    string line;

    // Skip first 4 lines: title, subtitle, empty line, and separator line
    for (int i = 0; i < 4; ++i) {
        if (!getline(fin, line)) {
            return; // file too short
        }
    }

    while (getline(fin, line)) {
        // Stop reading if separator line (starts with '+')
        if (!line.empty() && line[0] == '+') {
            break;
        }

        // Parse line split by '|'
        vector<string> columns;
        size_t start = 0, end;

        while ((end = line.find('|', start)) != string::npos) {
            string col = line.substr(start, end - start);
            col = trim(col);
            if (!col.empty()) {
                columns.push_back(col);
            }
            start = end + 1;
        }
        // last part after last '|'
        if (start < line.size()) {
            string col = trim(line.substr(start));
            if (!col.empty()) {
                columns.push_back(col);
            }
        }

        // Expect 4 columns: ID, NAME, JOINING DATE, TOTAL ASSIGNED
        if (columns.size() != 4) {
            cerr << "Warning: Skipping line due to unexpected columns (" << columns.size() << "): " << line << endl;
            continue;
        }

        Staff s;
        try {
            s.id = stoi(columns[0]);
            s.name = columns[1];
            s.joiningDate = columns[2];
            s.totalAssigned = stoi(columns[3]);
        } catch (const exception& e) {
            cerr << "Error parsing line: '" << line << "'\nReason: " << e.what() << endl;
            continue;
        }

        staffList.push_back(s);
    }

    fin.close();
}

void saveParcelData() {
    ofstream fout("parcels.txt");

    fout << centerAlign("Project Title: Parcel Delivery Optimizer") << "\n";
    fout << centerAlign("PARCEL RECORDS") << "\n\n";

    // Column widths
    const int wID = 3, wSender = 10, wReceiver = 10, wStatus = 10, wTime = 13;
    const int wDistance = 10, wWeight = 8, wLocation = 10, wAssigned = 8, wStaffId = 8;

    string separator = "+" + repeat('-', wID + 2) + "+" + repeat('-', wSender + 2) + "+" +
                       repeat('-', wReceiver + 2) + "+" + repeat('-', wStatus + 2) + "+" +
                       repeat('-', wTime + 2) + "+" + repeat('-', wDistance + 2) + "+" +
                       repeat('-', wWeight + 2) + "+" + repeat('-', wLocation + 2) + "+" +
                       repeat('-', wAssigned + 2) + "+" + repeat('-', wStaffId + 2) + "+";

    fout << separator << "\n";
    fout << "| " << setw(wID) << left << "ID"
         << " | " << setw(wSender) << left << "SENDER"
         << " | " << setw(wReceiver) << left << "RECEIVER"
         << " | " << setw(wStatus) << left << "STATUS"
         << " | " << setw(wTime) << left << "DELIVERY TIME"
         << " | " << setw(wDistance) << left << "DISTANCE"
         << " | " << setw(wWeight) << left << "WEIGHT"
         << " | " << setw(wLocation) << left << "LOCATION"
         << " | " << setw(wAssigned) << left << "ASSIGNED"
         << " | " << setw(wStaffId) << left << "STAFF ID" << " |\n";
    fout << separator << "\n";

    for (const Parcel& p : parcels) {
        fout << "| " << setw(wID) << left << p.id
             << " | " << setw(wSender) << left << p.sender
             << " | " << setw(wReceiver) << left << p.receiver
             << " | " << setw(wStatus) << left << p.status
             << " | " << setw(wTime) << left << p.deliveryTime
             << " | " << setw(wDistance) << left << p.distance
             << " | " << setw(wWeight) << left << p.weight
             << " | " << setw(wLocation) << left << p.location
             << " | " << setw(wAssigned) << left << (p.assigned ? "Yes" : "No")
             << " | " << setw(wStaffId) << left << p.assignedStaffId << " |\n";
    }

    fout << separator << "\n";
    fout.close();
}

void loadParcelData() {
    ifstream fin("parcels.txt");
    if (!fin.is_open()) return;

    parcels.clear();
    string line;

    // Skip first 4 lines (2 header + 2 separator lines)
    for (int i = 0; i < 4; ++i) {
        if (!getline(fin, line)) {
            // File ended early
            return;
        }
    }

    while (getline(fin, line)) {
        // Check for separator line (starts with '+'), stop loading if reached
        if (!line.empty() && line[0] == '+') {
            break;
        }

        // Each data line looks like:
        // | 1   | Alice    | Bob      | Pending | 15           | 120      | 5      | NY       | Yes      | 101      |
        // We'll split by '|', ignore empty parts, and trim spaces.

        vector<string> columns;
        size_t start = 0;
        size_t end;

        // Parse columns between '|' characters
        while ((end = line.find('|', start)) != string::npos) {
            string col = line.substr(start, end - start);
            col = trim(col);
            if (!col.empty()) {
                columns.push_back(col);
            }
            start = end + 1;
        }
        // Also check the last part after last '|'
        if (start < line.size()) {
            string col = trim(line.substr(start));
            if (!col.empty()) {
                columns.push_back(col);
            }
        }

        // Now columns vector should have exactly 10 columns for your fields
        if (columns.size() != 10) {
            cerr << "Warning: Skipping line due to unexpected number of columns (" << columns.size() << "): " << line << endl;
            continue;
        }

        Parcel p;
        try {
            p.id = stoi(columns[0]);
            p.sender = columns[1];
            p.receiver = columns[2];
            p.status = columns[3];
            p.deliveryTime = stoi(columns[4]);
            p.distance = stoi(columns[5]);
            p.weight = stoi(columns[6]);
            p.location = columns[7];
            // assigned is "Yes" or "No"
            string assignedStr = columns[8];
            if (assignedStr == "Yes" || assignedStr == "yes") {
                p.assigned = true;
            } else if (assignedStr == "No" || assignedStr == "no") {
                p.assigned = false;
            } else {
                throw runtime_error("Invalid assigned field value: " + assignedStr);
            }
            p.assignedStaffId = stoi(columns[9]);

        } catch (const exception& e) {
            cerr << "Error parsing line: '" << line << "'\nReason: " << e.what() << endl;
            continue; // skip invalid line
        }

        parcels.push_back(p);
    }

    fin.close();

    if (!parcels.empty()) {
        nextParcelId = parcels.back().id + 1;
    }
}

// Merge Sort (Distance)
void merge(vector<Parcel>& arr, int l, int m, int r) {
    vector<Parcel> left(arr.begin() + l, arr.begin() + m + 1);
    vector<Parcel> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size()) {
        if (left[i].distance < right[j].distance)
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(vector<Parcel>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Greedy (Delivery Time)
void greedyDeliveryOrder() {
    sort(parcels.begin(), parcels.end(), [](Parcel a, Parcel b) {
        return a.deliveryTime < b.deliveryTime;
    });
    centerText("Parcels Sorted by Earliest Delivery Time:");
    for (Parcel& p : parcels) {
        centerText("Parcel #" + to_string(p.id) + "  " + p.receiver + " | Time: " + to_string(p.deliveryTime));
    }
}
// Greedy Method: Assign parcels to staff based on earliest delivery time-> Earliest Deadline First (EDF) principle.
void greedyAssignParcels() {
    vector<Parcel*> unassignedParcels;

    // Collect unassigned parcels
    for (Parcel& p : parcels) {
        if (!p.assigned) {
            unassignedParcels.push_back(&p);
        }
    }

    // Sort unassigned parcels by delivery time
    sort(unassignedParcels.begin(), unassignedParcels.end(), [](Parcel* a, Parcel* b) {
        return a->deliveryTime < b->deliveryTime;
    });

    // Assign to staff in round-robin fashion
    int staffCount = staffList.size();
    if (staffCount == 0 || unassignedParcels.empty()) {
        centerText("No staff or parcels available for assignment.");
        return;
    }

    int staffIndex = 0;
    for (Parcel* parcel : unassignedParcels) {
        parcel->assigned = true;
        parcel->assignedStaffId = staffList[staffIndex].id;
        // ✅ Track how many parcels this staff member has
        staffList[staffIndex].totalAssigned++;

        staffIndex = (staffIndex + 1) % staffCount;
    }

    centerText(" Greedy assignment complete. Parcels assigned by earliest delivery time.");
    saveStaffData();    // <-- Add this here
    saveParcelData();
}

// Dynamic Programming (Knapsack)
void dpKnapsack(int capacity) {
    int n = parcels.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (parcels[i - 1].weight <= w)
                dp[i][w] = max(parcels[i - 1].weight + dp[i - 1][w - parcels[i - 1].weight], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    centerText("Max Total Weight Deliverable (Capacity " + to_string(capacity) + "): " + to_string(dp[n][capacity]));
}

// View Parcels
void viewParcels() {
    if (parcels.empty()) {
    centerText("No parcels available.");
    return;
}
    printLine();
    centerText("CURRENT PARCELS");
    printLine();
    for (Parcel& p : parcels) {
        string line = "ID: " + to_string(p.id) + " | Status: " + p.status + " | To: " + p.receiver;
        line += " | Distance: " + to_string(p.distance) + "km | Weight: " + to_string(p.weight) + "kg";
        if (p.assigned) {
            for (Staff& s : staffList)
                if (s.id == p.assignedStaffId)
                    line += " | Assigned to: " + s.name;
        } else {
            line += " | Not Assigned";
        }
        centerText(line);
    }
}

// Assign Staff
void assignStaff() {
    int parcelId, staffId;
    viewParcels();
    centerText("Enter Parcel ID to assign:");
    cin >> parcelId;

    centerText("Available Staff:");
    for (Staff& s : staffList)
        centerText(to_string(s.id) + "  " + s.name);

    centerText("Enter Staff ID to assign:");
    cin >> staffId;

    for (Parcel& p : parcels) {
    if (p.id == parcelId) {
        p.assigned = true;
        p.assignedStaffId = staffId;

        // Increment that staff’s parcel count
        for (Staff& s : staffList) {
            if (s.id == staffId) {
                s.totalAssigned++;
                break;
            }
        }

        centerText(" Assigned Staff ID " + to_string(staffId) + " to Parcel " + to_string(parcelId));
        saveStaffData();    // <-- Add this here
        saveParcelData();   // <-- Add this here
        return;
    }
}
    centerText(" Parcel not found.");
}

// Update by Staff
void updateStatus(int staffId) {
    int parcelId;
    viewParcels();
    centerText("Enter Parcel ID to update:");
    cin >> parcelId;

    for (Parcel& p : parcels) {
        if (p.id == parcelId && p.assigned && p.assignedStaffId == staffId) {
            cin.ignore();
            centerText("Enter new status (e.g., Picked/Delivered):");
            getline(cin, p.status);
            centerText("Enter current location:");
            getline(cin, p.location);
            centerText(" Status updated.");
            saveParcelData();
            return;
        }
    }
    centerText(" Parcel not found or not assigned to you.");
}

// User track
void userTrack() {
    int parcelId;
    centerText("Enter Parcel ID to track:");
    cin >> parcelId;

    for (Parcel& p : parcels) {
        if (p.id == parcelId) {
            centerText(" Status: " + p.status + " | Location: " + p.location);
            return;
        }
    }
    centerText("Parcel not found.");
}

// User cancel
void userCancel() {
    int parcelId;
    centerText("Enter Parcel ID to cancel:");
    cin >> parcelId;
    for (int i = 0; i < parcels.size(); i++) {
        if (parcels[i].id == parcelId && parcels[i].status == "Booked") {
            parcels.erase(parcels.begin() + i);
            centerText(" Parcel canceled.");
            return;
        }
    }
    centerText(" Cannot cancel parcel.");
}

// Add Dummy Data
void addTestData() {
    staffList.push_back({1, "Rafi"});
    staffList.push_back({2, "Nusrat"});
    parcels.push_back({nextParcelId++, "Alice", "Bob", "Booked", 3, 10, 5});
    parcels.push_back({nextParcelId++, "John", "Doe", "Booked", 1, 20, 3});
    parcels.push_back({nextParcelId++, "Lina", "Mina", "Booked", 2, 15, 4});
}
void addNewStaff() {
    int id;
    string name, joiningDate;
    centerText("Enter New Staff ID:");
    cin >> id;
    cin.ignore();

    centerText("Enter Staff Name:");
    getline(cin, name);

    centerText("Enter Joining Date (e.g., 2025-07-11):");
    getline(cin, joiningDate);

    staffList.push_back({id, name, joiningDate});
    centerText(" Staff Added: " + name);
    saveStaffData();
}


void addNewParcel() {
    string sender, receiver, status = "Booked";
    int time, dist, wt;
    centerText("Enter Sender Name:");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
    getline(cin, sender);

    centerText("Enter Receiver Name:");
    getline(cin, receiver);
    centerText("Enter Delivery Time (e.g., 3 for 3pm):");
    cin >> time;
    centerText("Enter Distance in km:");
    cin >> dist;
    centerText("Enter Weight in kg:");
    cin >> wt;
    parcels.push_back({nextParcelId++, sender, receiver, status, time, dist, wt});
    centerText(" Parcel Added Successfully.");
    saveParcelData();
}
void viewStaffDetails() {
    printLine();
    if (staffList.empty()) {
    centerText("No staff records found.");
    return;
}
    centerText("STAFF DETAILS");
    printLine();
    for (const Staff& s : staffList) {
        string info = "ID: " + to_string(s.id) + " | Name: " + s.name + " | Joined: " + s.joiningDate;
        int assignedCount = 0;

        // Count parcels assigned to this staff
        for (const Parcel& p : parcels) {
            if (p.assigned && p.assignedStaffId == s.id)
                assignedCount++;
        }

        info += " | Assigned Parcels: " + to_string(assignedCount);
        centerText(info);

        // List assigned parcel IDs
        for (const Parcel& p : parcels) {
            if (p.assigned && p.assignedStaffId == s.id) {
                centerText("  Parcel #" + to_string(p.id) + " | To: " + p.receiver + " | Time: " + to_string(p.deliveryTime));
            }
        }
    }
}


// Role Menus
void adminMenu() {
    int choice;
    do {
        system("cls");
        centerText(" ADMIN PANEL");
        printLine();
     centerList({
    "[1] View Parcels",
    "[2] Assign Staff",
    "[3] Sort Parcels by Distance",
    "[4] Optimize by Delivery Time",
    "[5] Optimize by Weight (Capacity 10kg)",
    "[6] Add New Parcel",
    "[7] Add New Staff",
    "[8] Greedy Assign Parcels by Delivery Time",
    "[9] View Staff Details",
    "[0] Logout"
});


        centerText("Enter your choice:");
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            centerText("Invalid input. Please enter a number.");
            continue;
        }

        switch (choice) {
            case 1: viewParcels(); break;
            case 2: assignStaff(); break;
            case 3: mergeSort(parcels, 0, parcels.size()-1); centerText(" Sorted by distance."); break;
            case 4: greedyDeliveryOrder(); break;
            case 5: dpKnapsack(10); break;
            case 6: addNewParcel(); break;
            case 7: addNewStaff(); break;
            case 8: greedyAssignParcels(); break;
            case 9: viewStaffDetails(); break;
        }


        if (choice != 0) {
            centerText("Press Enter to continue...");
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);
}

void staffMenu() {
    int staffId, choice;
    system("cls");
    centerText(" STAFF PANEL");
    centerText("Enter your Staff ID:");
    cin >> staffId;
    do {
        system("cls");
        centerText(" STAFF PANEL");
        centerList({
            "[1] Update Assigned Parcel Status",
            "[0] Logout"
        });
        centerText("Enter choice:");
        cin >> choice;
        if (choice == 1) updateStatus(staffId);
        if (choice != 0) {
            centerText("Press Enter to continue...");
            cin.ignore();
            cin.get();
        }
    } while (choice != 0);
}

void userMenu() {
    int choice;
    do {
        system("cls");
        centerText(" USER PANEL");
        centerList({
            "[1] Track Parcel",
            "[2] Cancel Parcel",
            "[0] Logout"
        });
        centerText("Enter your choice:");
        cin >> choice;
        if (choice == 1) userTrack();
        else if (choice == 2) userCancel();
        if (choice != 0) {
            centerText("Press Enter to continue...");
            cin.ignore();
            cin.get();
        }
    } while (choice != 0);
}

// Main Function
int main() {
    loadStaffData();
    loadParcelData();
    // addTestData(); // Optional — comment this if using saved data

    int role;
    while (true) {
        system("cls");
        centerText(" PARCEL DELIVERY OPTIMIZER");
        printLine();
        centerList({
            "[1] Admin Login",
            "[2] Delivery Staff Login",
            "[3] User Access",
            "[0] Exit"
        });
        centerText("Select your role:");
        cin >> role;
        switch (role) {
            case 1: adminMenu(); break;
            case 2: staffMenu(); break;
            case 3: userMenu(); break;
            case 0:
                saveStaffData();
                saveParcelData();
                centerText("Goodbye!");
                return 0;
            default: centerText(" Invalid option. Try again.");
        }
    }
}
