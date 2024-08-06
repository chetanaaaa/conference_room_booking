#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include<limits>
#include <regex>
#include<conio.h>
#include<ctype.h>
#include <cstdlib>
#include"C:\1-pbhat-Folders\Putti\C++\FinalProject\hall.cpp"

using namespace std;


// BookingSystem class
class BookingSystem {
private:
    vector<Customer> customers;
    vector<Hall> halls;
    vector<string> waitingList;
    Customer* loggedInUser; // To keep track of the currently logged-in user

public:

    BookingSystem() {
      
        // Load customers from file
        loadCustomers();
        loadHalls();
        loadWaitingList();

        loggedInUser = nullptr; // Initialize the logged-in user to nullptr
    }

    void signUp() {
        string username, password;
        std::cout << "Enter username: ";
        cin >> username;
        std::cout << "Enter password: ";
        // cin >> password;
         char ch = _getch();
          while(ch != 13){//character 13 is enter
      password.push_back(ch);
      std::cout << '*';
      ch = _getch();
        }
        for (auto& customer : customers) {
            if (customer.username == username) {
                std::cout << "\nUser name already exists try other user name!!" << endl;
                return;
            }
        }

        string startTime, endTime;
        std::cout << "\nEnter the aprrox starting time of the meeting (HH:MM): ";
        startTime = getTimeInput();
        std::cout << "\nEnter the approx ending time of the meeting (HH:MM): ";
        endTime = getTimeInput();

        Customer customer(username, password, startTime, endTime);
        customers.push_back(customer);

        std::cout << "\nSign up successful!" << endl;

        // Save customers to file
        saveCustomers();
    }

    void logIn() {
        string username, password;
        std::cout << "Enter username: ";
        cin >> username;
        std::cout << "Enter password: ";
        // cin >> password;
         char ch = _getch();
        while(ch != 13){//character 13 is enter
            password.push_back(ch);
            std::cout << '*';
            ch = _getch();
        }


        for (auto& customer : customers) {
            if (customer.username == username && customer.password == password) {
                std::cout <<endl<<"\nLogin successful!" << endl;
                loggedInUser = &customer; // Set the logged-in user
                return;
            }
        }

        std::cout << "\nInvalid username or password. Please try again." << endl;
    }

    void bookHall() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before booking a hall." << endl;
            return;
        }

        std::cout << "Available Halls:" << endl;
        displayHalls();
        int hallIndex;
        label:
        try{
        
        std::cout << "Enter the index of the hall you want to book: ";
        cin >> hallIndex;
        if(cin.fail()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw runtime_error("Invalid input\n");
            
        }
        

        if (hallIndex < 0 || hallIndex >= halls.size()) {
            std::cout << "Invalid hall index." << endl;
            return;
        }
        }

        catch(...){
            cout<<"Enter a valid number\n"<<endl;
            goto label;
        }

        Hall& selectedHall = halls[hallIndex];

        if (selectedHall.occupied) {
            std::cout << "Hall is already booked. Adding to waiting list." << endl;
            waitingList.push_back(loggedInUser->username);
             loggedInUser->meetingDate = getDateInput();
        loggedInUser->startTime = getTimeInput();
        loggedInUser->endTime = getTimeInput();
        } else {
            selectedHall.occupied = true;
             loggedInUser->hallname=to_string(hallIndex);
             selectedHall.hall_in=to_string(hallIndex);
              loggedInUser->meetingDate = getDateInput();
        loggedInUser->startTime = getTimeInput();
        loggedInUser->endTime = getTimeInput();
        int hh1=std::stoi(    loggedInUser->startTime .substr(0,2));
    int mm1=std::stoi(    loggedInUser->startTime .substr(3,2));
            
          int hh2=std::stoi(         loggedInUser->endTime.substr(0,2));
    int mm2=std::stoi(         loggedInUser->endTime.substr(3,2));
      int diff_minute = ( mm2 - mm1);
   int diff_hour = (hh2-hh1);
   if (diff_hour<0){
    diff_hour=12+diff_hour;
   }
   int duration=diff_minute+(diff_hour*60);
   int total = (duration*selectedHall.price)/30;
  
       

        cout << "Hall booked successfully!" << endl;
        cout<<"Your total bill is "<<total<<endl;

       Customer::objectCount++;
                 
        }  
//  std::cout << "Total Cost: $" << totalCost << endl;
        // Save customers and waiting list to file
        saveCustomers();
        saveWaitingList();
        saveHalls();
    }

    void cancelBooking() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before cancelling a booking." << endl;
            return;
        }

        if (loggedInUser->startTime.empty() || loggedInUser->endTime.empty()) {
            std::cout << "No booking found for the logged-in user." << endl;
            return;
        }

        Hall* bookedHall = getBookedHall();
        if (bookedHall != nullptr) {
            bookedHall->occupied = false;
            
        }

        loggedInUser->startTime.clear();
        loggedInUser->endTime.clear();

        std::cout << "Booking canceled successfully!" << endl;
         Customer::objectCount--;

        // Save customers to file
        saveCustomers();
        saveHalls();
    }

    void rescheduleBooking() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before rescheduling a booking." << endl;
            return;
        }

        if (loggedInUser->startTime.empty() || loggedInUser->endTime.empty()) {
            std::cout << "No booking found for the logged-in user." << endl;
            return;
        }

        std::cout << "Rescheduling Options:" << endl;
        std::cout << "1. Rebook the hall" << endl;
        std::cout << "2. Cancel the booking" << endl;
        std::cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cancelBooking();
                bookHall();
                break;
            case 2:
                cancelBooking();
                 
                break;
            default:
                std::cout << "Invalid choice." << endl;
                break;
        }
    }

    void displayHalls() {
        for (int i = 0; i < halls.size(); i++) {
            std::cout << "Hall " << i << ": " << halls[i].name << endl;
            std::cout << "AC: " << (halls[i].ac ? "Yes" : "No") << endl;
            std::cout << "Stage: " << (halls[i].stage ? "Yes" : "No") << endl;
            std::cout << "Speakers: " << (halls[i].speakers ? "Yes" : "No") << endl;
            std::cout << "Price: " << halls[i].price << endl;
            std::cout << "Occupied: " << (halls[i].occupied ? "Yes" : "No") << endl;
            std::cout << endl;
        }
    }

    void displayWaitingList() {
        if (waitingList.empty()) {
            std::cout << "Waiting list is empty." << endl;
            return;
        }

        std::cout << "Waiting List:" << endl;
        for (const auto& customer : waitingList) {
            std::cout << customer << endl;
        }
    }

private:
    void loadCustomers() {
        ifstream file("customers.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, '|');
            if (tokens.size() == 4) {
                string username = tokens[0];
                string password = tokens[1];
                string startTime = tokens[2];
                string endTime = tokens[3];

                Customer customer(username, password, startTime, endTime);
                customers.push_back(customer);
            }
        }

        file.close();
    }

    void loadHalls(){

             ifstream file("halls.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, '|');
            if (tokens.size() == 6) {
                string name = tokens[0];
                bool ac = (bool)stoi(tokens[1]);
                bool stage = (bool)stoi(tokens[2]);
                bool speakers = (bool)stoi(tokens[3]);
                double price = stoi(tokens[4]);
                bool occupied = (bool)stoi(tokens[5]);

                Hall hall(name,ac,stage,speakers,price,occupied);
                halls.push_back(hall);
            }
        }

        file.close();
        }


    void saveHalls(){
        ofstream file("halls.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& hall : halls) {
            file << hall.name << "|" << hall.ac << "|"
                 << hall.stage << "|" << hall.speakers<<"|" <<hall.price<<"|"<<hall.occupied<< endl;
        }

        file.close();
    }

    void saveCustomers() {
        ofstream file("customers.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& customer : customers) {
            file << customer.username << "|" << customer.password << "|"
                 << customer.startTime << "|" << customer.endTime << endl;
        }

        file.close();
    }

    void loadWaitingList() {
        ifstream file("waiting_list.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            waitingList.push_back(line);
        }

        file.close();
    }

    void saveWaitingList() {
        ofstream file("waiting_list.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& customer : waitingList) {
            file << customer << endl;
        }

        file.close();
    }

    string getTimeInput() {
        string time;
        while (true) {
            std::cout << "Enter the time (HH:MM): ";
            cin >> time;

            regex pattern("(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]");
            if (regex_match(time, pattern)) {
                return time;
            } else {
                std::cout << "Invalid time format. Please enter in HH:MM format." << endl;
            }
        }
    }
string getDateInput() {
        string date;
        while (true) {
            std::cout << "Enter the date of the meeting (dd/mm/yyyy): ";
            cin >> date;

            regex pattern("([0-2][0-9]|3[0-1])/(0[1-9]|1[0-2])/\\d{4}");  // dd/mm/yyyy format
            if (regex_match(date, pattern)) {
                // Validate the entered date against the current date
                time_t currentTime = time(nullptr);
                tm* currentDate = localtime(&currentTime);
                int currentDay = currentDate->tm_mday;
                int currentMonth = currentDate->tm_mon + 1;
                int currentYear = currentDate->tm_year + 1900;

                int enteredDay = stoi(date.substr(0, 2));
                int enteredMonth = stoi(date.substr(3, 2));
                int enteredYear = stoi(date.substr(6, 4));

                if (enteredYear > currentYear ||
                    (enteredYear == currentYear && enteredMonth > currentMonth) ||
                    (enteredYear == currentYear && enteredMonth == currentMonth && enteredDay > currentDay)) {
                    return date;
                } else {
                    std::cout << "Invalid date. Please enter a future date." << endl;
                }
            } else {
                std::cout << "Invalid date format. Please enter in dd/mm/yyyy format." << endl;
            }
        }
    }

    vector<string> splitString(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    Hall* getBookedHall() {
        for (auto& hall : halls) {
            if (hall.hall_in == loggedInUser->hallname) {
                return &hall;
            }
        }
        return nullptr;
    }
};




