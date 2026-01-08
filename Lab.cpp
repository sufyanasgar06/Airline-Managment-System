#include<iostream>
#include<string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <fstream>
using namespace std;

struct Date {
    int day;
    int month;
    int year;
};
struct Time {
    int hour;
    int minute;
};

struct Passenger {
    int id;
    char name[50];
    char password[30];
    char email[50];
    char phone[15];
    int totalBookings;
    float totalSpent;
};

struct Flight {
    int flightNo;
    char destination[50];
    char origin[50];
    Date departureDate;
    Time departureTime;
    Date arrivalDate;
    Time arrivalTime;
    int economySeats;
    int businessSeats;
    int firstClassSeats;
    float economyFare;
    float businessFare;
    float firstClassFare;
    int totalSeats;
    int availableSeats;
    float distance;
    char status[20];
    int timesBooked;
    float totalRevenue;
    double baseFare;
};

struct Booking {
    int bookingId;
    int passengerId;
    int flightNo;
    Date bookingDate;
    Date travelDate;
    int seatsBooked;
    char classType[20];
    float farePaid;
    char status[20]; 
};


int MAX_PASSENGERS = 100;  
int MAX_FLIGHTS = 100;         
int MAX_BOOKINGS = 100; 

Passenger* passengers = nullptr; 
Flight* flights = nullptr;         
Booking* bookings = nullptr; 

int passengerCount = 0;
int flightCount = 0;
int bookingCount = 0;
int currentPassengerId = -1;

// Function prototypes passenger(Shoaib)

bool isValidDate(const Date& date);
bool isValidEmail(const char* email);
void viewAvailableFlights();
void bookFlight();
void showPassengerMenu();
void passengerLogin();
void PassengerRegistration();
void mainMenu();
int generateBookingId();
void viewFlightDetailsWithSeats();
void cancelBooking();

// Function prototypes passenger(Mubeen)

float calculateFare(const Flight& flight, int seats, const string& classType);
float calculateRefundAmount(const Booking& booking);
void displayFareBreakdown(const Flight& flight, int seats, const string& classType);
Booking* findBookingById(int bookingId, int& index);
Flight* findFlightByNumber(int flightNo, int& index);
void displayPassengerBookings();
void generateBookingReceipt(int bookingId);
void generatePersonalReport();

// File handling prototypes(Ehsan)
void loadPassengers();
void savePassengers();
void loadFlights();
void saveFlights();
void loadBookings();
void saveBookings();

// Admin functions(Sufyan + hasnat)
void adminMenu();
void adminLoginPanel();
void addFlight(Flight flights[], int &flightCount);
void viewFlights(Flight flights[], int flightCount);
void updateFlight(Flight flights[], int flightCount);
void deleteFlight(Flight flights[], int &flightCount);
void viewAllBookings();
void updatePassengerAdmin();
void removePassengerAdmin();

// ========== VALIDATION FUNCTIONS ==========

bool isValidDate(const Date& date) {
    if (date.day < 1 || date.day > 31) return false;
    if (date.month < 1 || date.month > 12) return false;
    if (date.year < 2024) return false;
    return true;
}

bool isValidEmail(const char* email) {
    int atCount = 0;
    int dotCount = 0;
    int len = strlen(email);
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    
    return (atCount == 1 && dotCount >= 1 && len >= 5);
}

bool isValidDate(int day, int month, int year) {
    if (year < 2024 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int maxDays;
    if (month == 2)
        maxDays = (year % 4 == 0) ? 29 : 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        maxDays = 30;
    else
        maxDays = 31;

    return (day >= 1 && day <= maxDays);
}

bool isValidTime(int hour, int minute) {
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

// ========== FORMATTING FUNCTIONS ==========

string formatDate(const Date& date) {
    return to_string(date.day) + "/" + 
           to_string(date.month) + "/" + 
           to_string(date.year);
}

string formatTime(const Time& time) {
    string hour = to_string(time.hour);
    string minute = (time.minute < 10) ? "0" + to_string(time.minute) : to_string(time.minute);
    return hour + ":" + minute;
}

// ========== VIEW FUNCTIONS ==========


void viewAvailableFlights() {
    cout << "\n=== AVAILABLE FLIGHTS ===\n";
    
    if (flightCount == 0) {
        cout << "Sorry! No flights available at the moment.\n";
        return;
    }
    
    cout << left << setw(8) << "Flight #"<< setw(12) << "From" << setw(12) << "To"<< setw(10) << "Date"<< setw(8) << "Time"<< setw(12) << "Economy"<< setw(12) << "Business"<< setw(12) << "First class"<< setw(12) << "Status" << "\n";
    cout << string(100, '-') << "\n";
    
    bool hasAvailable = false;
    for (int i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].status, "Available") == 0) {
            hasAvailable = true;
            int ecoSeats = flights[i].economySeats;
            int busSeats = flights[i].businessSeats;
            int firstSeats = flights[i].firstClassSeats;
            string dateStr = to_string(flights[i].departureDate.day) + "/" +
                           to_string(flights[i].departureDate.month);
            string timeStr = to_string(flights[i].departureTime.hour) + ":" +
                           (flights[i].departureTime.minute < 10 ? "0" : "") +
                           to_string(flights[i].departureTime.minute);
            cout << left << setw(8) << flights[i].flightNo<< setw(12) << flights[i].origin<< setw(12) << flights[i].destination<< setw(10) << dateStr<< setw(8) << timeStr<< setw(12) << ecoSeats<< setw(12) << busSeats<< setw(12) << firstSeats<< setw(12) << flights[i].status << "\n";
        }
    }
    
    if (!hasAvailable) {
        cout << "No available flights at the moment.\n";
    }
    
}


void viewFlights(Flight flights[], int flightCount) {
    if (flightCount == 0) {
        cout << "\nNo flights available.\n";
        return;
    }

    cout << "\n========== AVAILABLE FLIGHTS ==========\n";

    for (int i = 0; i < flightCount; i++) {
        cout << "\n--------------------------------------\n";
        cout << "Flight Number   : " << flights[i].flightNo << endl;
        cout << "Origin          : " << flights[i].origin << endl;
        cout << "Destination     : " << flights[i].destination << endl;
        cout << "Departure Date  : " << flights[i].departureDate.day << "/"
             << flights[i].departureDate.month << "/" << flights[i].departureDate.year << endl;
        cout << "Departure Time  : " << flights[i].departureTime.hour << ":"
             << flights[i].departureTime.minute << endl;
        cout << "Arrival Date    : " << flights[i].arrivalDate.day << "/"
             << flights[i].arrivalDate.month << "/" << flights[i].arrivalDate.year << endl;
        cout << "Arrival Time    : " << flights[i].arrivalTime.hour << ":"
             << flights[i].arrivalTime.minute << endl;
        cout << "\n--- Seats & Fares ---\n";
        cout << "Economy Seats   : " << flights[i].economySeats
             << " | Fare: " << flights[i].economyFare << endl;
        cout << "Business Seats  : " << flights[i].businessSeats
             << " | Fare: " << flights[i].businessFare << endl;
        cout << "First Class Seats: " << flights[i].firstClassSeats
             << " | Fare: " << flights[i].firstClassFare << endl;
        cout << "\nTotal Seats     : " << flights[i].totalSeats << endl;
        cout << "Available Seats : " << flights[i].availableSeats << endl;
        cout << "Distance        : " << flights[i].distance << " km" << endl;
        cout << "Status          : " << flights[i].status << endl;
        cout << "Times Booked    : " << flights[i].timesBooked << endl;
        cout << "Total Revenue   : " << flights[i].totalRevenue << endl;
    }
    cout << "\n======================================\n";
}

void viewFlightDetailsWithSeats() {
    cout << "\n=== FLIGHT DETAILS WITH SEAT AVAILABILITY ===\n";
    
    if (flightCount == 0) {
        cout << "No flights available.\n";
        return;
    }
    
    int flightNo;
    cout << "Enter Flight Number to view details (0 to cancel): ";
    cin >> flightNo;
    
    if (flightNo == 0) return;
    
    Flight* flight = nullptr;
    int flightIndex = -1;
    
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            flight = &flights[i];
            flightIndex = i;
            break;
        }
    }
    
    if (!flight) {
        cout << "Flight not found!\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "     FLIGHT DETAILS: " << flight->flightNo << "\n";
    cout << "========================================\n";
    
    cout << "\nRoute: " << flight->origin << " to " << flight->destination << "\n";
    cout << "Distance: " << flight->distance << " km\n";
    
    cout << "\n--- Departure ---\n";
    cout << "Date: " << formatDate(flight->departureDate) << "\n";
    cout << "Time: " << formatTime(flight->departureTime) << "\n";
    
    cout << "\n--- Arrival ---\n";
    cout << "Date: " << formatDate(flight->arrivalDate) << "\n";
    cout << "Time: " << formatTime(flight->arrivalTime) << "\n";
    
    cout << "\n--- SEAT AVAILABILITY ---\n";
    cout << left << setw(15) << "CLASS" 
        //  << setw(10) << "SEATS" 
         << setw(15) << "AVAILABLE" 
         << setw(15) << "FARE PER SEAT" << "\n";
    cout << string(55, '-') << "\n";
    
    // Calculate fares per seat based on distance
    float distanceFare = flight->distance * (flight->baseFare / 100);
    
    cout << left 
         << setw(15) << "Economy"
         << setw(15) << flight->economySeats
        //  << setw(15) << flight->economySeats
         << setw(15) << "$" + to_string(distanceFare * flight->economyFare) << "\n";
    
    cout << left 
         << setw(15) << "Business"
        //  << setw(10) << flight->businessSeats
         << setw(15) << flight->businessSeats
         << setw(15) << "$" + to_string(distanceFare * flight->businessFare) << "\n";
    
    cout << left 
         << setw(15) << "First Class"
        //  << setw(10) << flight->firstClassSeats
         << setw(15) << flight->firstClassSeats
         << setw(15) << "$" + to_string(distanceFare * flight->firstClassFare) << "\n";
    
    cout << "\nTotal Seats: " << flight->totalSeats << "\n";
    cout << "Status: " << flight->status << "\n";
    cout << "========================================\n";
}
// ========== BOOKING FUNCTIONS ==========

void bookFlight() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n=== BOOK A FLIGHT ===\n";
    viewAvailableFlights();
    
    if (flightCount == 0) {
        cout << "No flights available to book.\n";
        return;
    }
       if (bookingCount >= MAX_BOOKINGS) {
        cout << "Maximum bookings limit reached!\n";
        cout << "Cannot create new booking.\n";
        return;
    }
    
    int flightChoice;
    cout << "\nEnter Flight Number to book (0 to cancel): ";
    cin >> flightChoice;
    
    if (flightChoice == 0) return;
    
    Flight* selectedFlight = nullptr;
    int flightIndex = -1;
    
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightChoice && 
            strcmp(flights[i].status, "Available") == 0) {
            selectedFlight = &flights[i];
            flightIndex = i;
            break;
        }
    }
    if (!selectedFlight) {
        cout << "Invalid flight selection or flight not available!\n";
        return;
    }
    if (selectedFlight->availableSeats <= 0) {
        cout << "Sorry, this flight is fully booked!\n";
        return;
    }
    int seats;
    cout << "Number of seats to book (1-" << selectedFlight->availableSeats << "): ";
    cin >> seats;
    
    while (seats < 1 || seats > selectedFlight->availableSeats) {
        cout << "Invalid! Enter between 1 and " << selectedFlight->availableSeats << " seats: ";
        cin >> seats;
    }
    
    cout << "\nSelect Class Type:\n";
    cout << "1. Economy \n";
    cout << "2. Business \n";
    cout << "3. First Class\n";
    
    int classChoice;
    cout << "Enter choice (1-3): ";
    cin >> classChoice;
    
    string classType;
    int classSeatsAvailable = 0;
    
    switch(classChoice) {
        case 1: 
            classType = "Economy";
            classSeatsAvailable = selectedFlight->economySeats;
            break;
        case 2: 
            classType = "Business";
            classSeatsAvailable = selectedFlight->businessSeats;
            break;
        case 3: 
            classType = "First";
            classSeatsAvailable = selectedFlight->firstClassSeats;
            break;
        default: 
            cout << "Invalid choice! Defaulting to Economy.\n";
            classType = "Economy";
            classSeatsAvailable = selectedFlight->economySeats;
            break;
    }
    
    if (seats > classSeatsAvailable) {
        cout << "Sorry! Only " << classSeatsAvailable << " seats available in " << classType << " class.\n";
        return;
    }
    Date travelDate;
    cout << "\nEnter travel date (DD MM YYYY): ";
    cin >> travelDate.day >> travelDate.month >> travelDate.year;
    if (!isValidDate(travelDate)) {
        cout << "Invalid date!\n";
        return;
    }    
    float fare = calculateFare(*selectedFlight, seats, classType);

    cout << "\n=== BOOKING SUMMARY ===\n";
    cout << "Flight: " << selectedFlight->origin << " to " << selectedFlight->destination << "\n";
    cout << "Date: " << travelDate.day << "/" << travelDate.month << "/" << travelDate.year << "\n";
    cout << "Seats: " << seats << " (" << classType << " class)\n";
    
    displayFareBreakdown(*selectedFlight, seats, classType);
    
    cout << "Total Fare: $" << fixed << setprecision(2) << fare << "\n";

    char confirm;
    cout << "\nConfirm booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Booking cancelled.\n";
        return;
    }
   
    Booking newBooking;
    newBooking.bookingId = generateBookingId();
    newBooking.passengerId = currentPassengerId;
    newBooking.flightNo = flightChoice;
    
    // Set booking date to current date
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    newBooking.bookingDate.day = currentTime->tm_mday;
    newBooking.bookingDate.month = currentTime->tm_mon + 1;
    newBooking.bookingDate.year = currentTime->tm_year + 1900;
    
    newBooking.travelDate = travelDate;
    newBooking.seatsBooked = seats;
    strcpy(newBooking.classType, classType.c_str());
    newBooking.farePaid = fare;
    strcpy(newBooking.status, "Confirmed");
    

    if (bookingCount < MAX_BOOKINGS) {
        bookings[bookingCount] = newBooking;
        bookingCount++;
    } else {
        cout << "Error: Maximum bookings limit reached!\n";
        return;
    }
    
 
    if (flightIndex >= 0 && flightIndex < flightCount) {
        // Reduce seats 
        if (classType == "Economy") {
            flights[flightIndex].economySeats -= seats;
        } else if (classType == "Business") {
            flights[flightIndex].businessSeats -= seats;
        } else if (classType == "First") {
            flights[flightIndex].firstClassSeats -= seats;
        }
        
        // Update total available seats
        flights[flightIndex].availableSeats -= seats;
                
        // Update flight status if fully booked
        if (flights[flightIndex].availableSeats == 0) {
            strcpy(flights[flightIndex].status, "Full");
        }
    }
    
    // UPDATE PASSENGER INFORMATION
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalBookings++;
            passengers[i].totalSpent += fare;
            break;
        }
    }
    
    // Show confirmation and generate receipt
    cout << "\n Booking confirmed! Booking ID: " << newBooking.bookingId << "\n";
    cout << " Generating receipt...\n\n";
    

    generateBookingReceipt(newBooking.bookingId);
    
    cout << "\n IMPORTANT: Save your Booking ID: " << newBooking.bookingId << "\n";
    cout << "You can view this receipt anytime from 'View Booking Receipt' in menu.\n";
    flights[flightIndex].timesBooked++;
    flights[flightIndex].totalRevenue += fare;
    saveBookings();
    saveFlights();
}

//=========================Passenger Regestration And Login===================================

void passengerLogin() {
    cout << "\n=== PASSENGER LOGIN ===\n";
    
    int id;
    char password[30];
    
    cout << "Enter Passenger ID: ";
    cin >> id;
    cin.ignore();
    
    cout << "Enter Password: ";
    cin.getline(password, 30);
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == id && strcmp(passengers[i].password, password) == 0) {
            currentPassengerId = id;
            cout << "\nLogin successful! Welcome " << passengers[i].name << "!\n";
            showPassengerMenu();
            return;
        }
    }
    
    cout << "Invalid credentials! Please try again.\n";
}

void PassengerRegistration() {
   if (passengerCount >= MAX_PASSENGERS) {
       cout << "Maximum passenger limit reached!\n";
        cout << "Cannot register more passengers.\n";
        return;
    }

    Passenger newPassenger;
    
    cout << "\n=== PASSENGER REGISTRATION ===\n";
    
    newPassenger.id = 1000 + passengerCount + 1;
    cout << "Your Passenger ID: " << newPassenger.id << " (Remember this for login)\n";
    
    cout << "Enter your name: ";
    cin.ignore();
    cin.getline(newPassenger.name, 50);
    
    cout << "Enter your Password: ";
    cin.getline(newPassenger.password, 30);
    
    char email[50];
    while (true) {
        cout << "Enter E-mail: ";
        cin.getline(email, 50);
        if (isValidEmail(email)) {
            strcpy(newPassenger.email, email);
            break;
        } else {
            cout << "Invalid email format! Please enter a valid email (e.g., user@domain.com)\n";
        }
    }
    
    cout << "Enter Phone Number: ";
    cin.getline(newPassenger.phone, 15);
    
    newPassenger.totalBookings = 0;
    newPassenger.totalSpent = 0.0;
    
    passengers[passengerCount] = newPassenger;
    passengerCount++;
    
    cout << "\nRegistration successful!\n";
    cout << "\n=== REGISTRATION DETAILS ===\n";
    cout << "Passenger ID: " << newPassenger.id << "\n";
    cout << "Name: " << newPassenger.name << "\n";
    cout << "Email: " << newPassenger.email << "\n";
    cout << "Phone: " << newPassenger.phone << "\n";
    cout << "\nIMPORTANT: Save your Passenger ID for login: " << newPassenger.id << "\n";
    savePassengers();
}

//================Genrate Booking ID====================
int generateBookingId() {
    static int lastId = 0;
    return ++lastId;
}

//================Cancel Booking========================

void cancelBooking() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    cout << "\n=== CANCEL BOOKING ===\n";
    displayPassengerBookings();
    bool hasBookings = false;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId && 
            strcmp(bookings[i].status, "Confirmed") == 0) {
            hasBookings = true;
            break;
        }
    }
    if (!hasBookings) {
        cout << "\nYou have no active bookings to cancel.\n";
        return;
    }
    
    int bookingId;
    cout << "\nEnter Booking ID to cancel (0 to go back): ";
    cin >> bookingId;
    
    if (bookingId == 0) return;
    
    int bookingIndex = -1;
    Booking* bookingToCancel = findBookingById(bookingId, bookingIndex);
    
    if (!bookingToCancel) {
        cout << "Invalid Booking ID or booking not found!\n";
        return;
    }
    
    if (strcmp(bookingToCancel->status, "Cancelled") == 0) {
        cout << "This booking is already cancelled.\n";
        return;
    }
    
    if (strcmp(bookingToCancel->status, "Completed") == 0) {
        cout << "Cannot cancel a completed booking.\n";
        return;
    }
    
    float refundAmount = calculateRefundAmount(*bookingToCancel);
    
    cout << "\n=== CANCELLATION DETAILS ===\n";
    cout << "Booking ID: " << bookingToCancel->bookingId << "\n";
    cout << "Flight Number: " << bookingToCancel->flightNo << "\n";
    cout << "Travel Date: " << bookingToCancel->travelDate.day << "/" 
         << bookingToCancel->travelDate.month << "/" 
         << bookingToCancel->travelDate.year << "\n";
    cout << "Seats: " << bookingToCancel->seatsBooked << "\n";
    cout << "Class: " << bookingToCancel->classType << "\n";
    cout << "Original Fare: $" << fixed << setprecision(2) << bookingToCancel->farePaid << "\n";
    cout << "Refund Amount: $" << fixed << setprecision(2) << refundAmount << "\n";
    
    if (refundAmount == 0) {
        cout << "Note: No refund as cancellation is too close to travel date.\n";
    }
    
    char confirm;
    cout << "\nAre you sure you want to cancel this booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Cancellation cancelled.\n";
        return;
    }
    
    int flightIndex = -1;
    Flight* flight = findFlightByNumber(bookingToCancel->flightNo, flightIndex);
    
    if (flight) {
        flight->availableSeats += bookingToCancel->seatsBooked;
        if (strcmp(flight->status, "Full") == 0) {
            strcpy(flight->status, "Available");
        }
        flight->totalRevenue -= refundAmount;
        flight->timesBooked--;
    }
    
    strcpy(bookingToCancel->status, "Cancelled");
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalSpent -= refundAmount;
            passengers[i].totalBookings--;
            break;
        }
    }
    
    cout << "\n=== CANCELLATION SUCCESSFUL ===\n";
    cout << "Booking ID " << bookingToCancel->bookingId << " has been cancelled.\n";
    
    if (refundAmount > 0) {
        cout << "Refund of $" << fixed << setprecision(2) << refundAmount 
             << " will be processed to your account.\n";
    }
    saveBookings();
    flight->totalRevenue -= bookingToCancel->farePaid;
    flight->timesBooked--;
    
}


// ========== CALCULATION FUNCTIONS ==========

void displayFareBreakdown(const Flight& flight, int seats, const string& classType) {
    float multiplier;
    
    if (classType == "Economy") multiplier=flight.economyFare;
    else if (classType == "Business") multiplier=flight.businessFare;
    else if (classType == "First") multiplier=flight.firstClassFare;
    else multiplier = flight.economyFare;
    
    float farePerSeat =  flight.distance* multiplier;
    float totalFare = farePerSeat * seats;
    
    cout << "\n=== FARE BREAKDOWN ===\n";
    cout << "Distance: " << flight.distance << " km\n";
    cout << "Rate per km: $" << fixed << setprecision(2) << multiplier << "\n";
    cout << "Class: " << classType << " (Multiplier: " << multiplier << "x)\n";
    cout << "Fare per seat: $" << farePerSeat << "\n";
    cout << "Number of seats: " << seats << "\n";
    cout << "TOTAL FARE (for all seats): $" << totalFare << "\n";
    cout << "=======================\n";
}

float calculateFare(const Flight& flight, int seats, const string& classType) {

    float multiplier;
    if (classType == "Economy") multiplier = flight.economyFare;
    else if (classType == "Business") multiplier = flight.businessFare;
    else if (classType == "First") multiplier = flight.firstClassFare;
    else multiplier =flight.economyFare;
    return flight.distance * multiplier * seats;
}


float calculateRefundAmount(const Booking& booking) {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    Date currentDate;
    currentDate.day = currentTime->tm_mday;
    currentDate.month = currentTime->tm_mon + 1;
    currentDate.year = currentTime->tm_year + 1900;

    int daysBeforeTravel =
        (booking.travelDate.year - currentDate.year) * 365 +
        (booking.travelDate.month - currentDate.month) * 30 +
        (booking.travelDate.day - currentDate.day);
    if (daysBeforeTravel >= 7)
        return booking.farePaid * 0.90;
    else if (daysBeforeTravel >= 3)
        return booking.farePaid * 0.50;
    else if (daysBeforeTravel >= 1)
        return booking.farePaid * 0.20;
    else
        return 0.0;
}

// ========== SEARCH FUNCTIONS ==========

Booking* findBookingById(int bookingId, int& index) {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].bookingId == bookingId && 
            bookings[i].passengerId == currentPassengerId) {
            index = i;
            return &bookings[i];
        }
    }
    return nullptr;
}

Flight* findFlightByNumber(int flightNo, int& index) {
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            return &flights[i];
        }
    }
    return nullptr;
}

void displayPassengerBookings() {
    cout << "\n===== YOUR BOOKINGS =====\n";
    bool found = false;
    cout << left << setw(12) << "Booking ID"
         << setw(10) << "Flight #"
         << setw(15) << "Travel Date"
         << setw(10) << "Seats"
         << setw(12) << "Class"
         << setw(12) << "Fare Paid($)"
         << setw(15) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            found = true;
            string travelDateStr = to_string(bookings[i].travelDate.day) + "/" +to_string(bookings[i].travelDate.month) + "/" +to_string(bookings[i].travelDate.year);
            cout << left << setw(12) << bookings[i].bookingId<< setw(10) << bookings[i].flightNo<< setw(15) << travelDateStr<< setw(10) << bookings[i].seatsBooked<< setw(12) << bookings[i].classType<< setw(12) << fixed << setprecision(2) << bookings[i].farePaid<< setw(15) << bookings[i].status << "\n";
        }
    }
    if (!found) {
        cout << "No bookings found.\n";
    }
}


// ========== RECEIPT GENERATION FUNCTION ==========

void generateBookingReceipt(int bookingId) {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    // Find the booking
    int bookingIndex = -1;
    Booking* booking = nullptr;
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].bookingId == bookingId && 
            bookings[i].passengerId == currentPassengerId) {
            booking = &bookings[i];
            bookingIndex = i;
            break;
        }
    }
    
    if (!booking) {
        cout << "Booking not found!\n";
        return;
    }
    
    // Find the flight
    Flight* flight = nullptr;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == booking->flightNo) {
            flight = &flights[i];
            break;
        }
    }
    
    if (!flight) {
        cout << "Flight information not found!\n";
        return;
    }
    
    // Find passenger
    Passenger* passenger = nullptr;
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passenger = &passengers[i];
            break;
        }
    }
    
    if (!passenger) {
        cout << "Passenger information not found!\n";
        return;
    }
    
    // Generate receipt
    cout << "\n========================================\n";
    cout << "         FLIGHT BOOKING RECEIPT\n";
    cout << "========================================\n\n";
    
    cout << "RECEIPT #: " << booking->bookingId << "\n";
    cout << "ISSUE DATE: " << formatDate(booking->bookingDate) << "\n";
    cout << "TIME: " << formatTime({12, 0}) << " (System Time)\n\n"; // You can add actual time
    
    cout << "------------------------------------------------\n";
    cout << "PASSENGER INFORMATION:\n";
    cout << "------------------------------------------------\n";
    cout << "Passenger ID: " << passenger->id << "\n";
    cout << "Name: " << passenger->name << "\n";
    cout << "Email: " << passenger->email << "\n";
    cout << "Phone: " << passenger->phone << "\n\n";
    
    cout << "------------------------------------------------\n";
    cout << "FLIGHT INFORMATION:\n";
    cout << "------------------------------------------------\n";
    cout << "Flight Number: " << flight->flightNo << "\n";
    cout << "Route: " << flight->origin << " to " << flight->destination << "\n";
    cout << "Departure: " << formatDate(flight->departureDate) << " at " 
         << formatTime(flight->departureTime) << "\n";
    cout << "Arrival: " << formatDate(flight->arrivalDate) << " at " 
         << formatTime(flight->arrivalTime) << "\n";
    cout << "Distance: " << flight->distance << " km\n\n";
    
    cout << "------------------------------------------------\n";
    cout << "BOOKING DETAILS:\n";
    cout << "------------------------------------------------\n";
    cout << "Travel Date: " << formatDate(booking->travelDate) << "\n";
    cout << "Class: " << booking->classType << "\n";
    cout << "Seats Booked: " << booking->seatsBooked << "\n";    
    cout << "------------------------------------------------\n";    
    cout << "TOTAL FARE: $" << fixed << setprecision(2) << booking->farePaid << "\n\n";
    
    cout << "------------------------------------------------\n";
    cout << "BOOKING STATUS: " << booking->status << "\n";
    cout << "------------------------------------------------\n\n";
    
    cout << "Terms & Conditions:\n";
    cout << "1. This receipt is proof of booking.\n";
    cout << "2. Cancellation charges apply as per policy.\n";
    cout << "3. Please arrive 2 hours before departure.\n";
    cout << "4. Carry valid ID proof for verification.\n\n";
    
    cout << "========================================\n";
    cout << "     Thank you for choosing our airline!\n";
    cout << "========================================\n";
}

// ========== REPORT FUNCTIONS ==========

int countBookingsByStatus(const char* status) {
    int count = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId && 
            strcmp(bookings[i].status, status) == 0) {
            count++;
        }
    }
    return count;
}

float getTotalSpentOnBookings() {
    float total = 0.0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            total += bookings[i].farePaid;
        }
    }
    return total;
}

void displayPassengerInfo() {
    cout << "\n=== PASSENGER INFORMATION ===\n";
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            cout << "Passenger ID: " << passengers[i].id << "\n";
            cout << "Name: " << passengers[i].name << "\n";
            cout << "Email: " << passengers[i].email << "\n";
            cout << "Phone: " << passengers[i].phone << "\n";
            cout << "Total Bookings: " << passengers[i].totalBookings << "\n";
            cout << "Total Spent: $" << fixed << setprecision(2) << passengers[i].totalSpent << "\n";
            break;
        }
    }
    cout << "------------------------------\n";
}

void displayBookingSummary() {
    cout << "\n=== BOOKING SUMMARY ===\n";
    
    int confirmed = countBookingsByStatus("Confirmed");
    int cancelled = countBookingsByStatus("Cancelled");
    int totalBookings = confirmed + cancelled;
    float totalSpent = getTotalSpentOnBookings();
    
    cout << "Total Bookings: " << totalBookings << "\n";
    cout << "Active Bookings: " << confirmed << "\n";
    cout << "Cancelled Bookings: " << cancelled << "\n";
    cout << "Total Amount Spent: $" << fixed << setprecision(2) << totalSpent << "\n";
    cout << "------------------------------\n";
}

void displayBookingHistory() {
    cout << "\n=== BOOKING HISTORY ===\n";
    
    bool hasBookings = false;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            hasBookings = true;
            break;
        }
    }
    
    if (!hasBookings) {
        cout << "No booking history found.\n";
        return;
    }
    
    cout << left << setw(12) << "Booking ID" 
         << setw(10) << "Flight #" 
         << setw(12) << "Book Date" 
         << setw(12) << "Travel Date" 
         << setw(8) << "Seats" 
         << setw(10) << "Class" 
         << setw(10) << "Fare($)" 
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            string bookDate = formatDate(bookings[i].bookingDate);
            string travelDate = formatDate(bookings[i].travelDate);
            
            cout << left << setw(12) << bookings[i].bookingId
                 << setw(10) << bookings[i].flightNo
                 << setw(12) << bookDate
                 << setw(12) << travelDate
                 << setw(8) << bookings[i].seatsBooked
                 << setw(10) << bookings[i].classType
                 << setw(10) << fixed << setprecision(2) << bookings[i].farePaid
                 << setw(12) << bookings[i].status << "\n";
        }
    }
    cout << "------------------------------\n";
}

void displayRecentBookings() {
    cout << "\n=== RECENT BOOKINGS ===\n";
    
    int count = 0;
    for (int i = bookingCount - 1; i >= 0 && count < 3; i--) {
        if (bookings[i].passengerId == currentPassengerId) {
            count++;
            string origin = "Unknown";
            string destination = "Unknown";
            
            for (int j = 0; j < flightCount; j++) {
                if (flights[j].flightNo == bookings[i].flightNo) {
                    origin = flights[j].origin;
                    destination = flights[j].destination;
                    break;
                }
            }
            
            string travelDate = formatDate(bookings[i].travelDate);
            
            cout << count << ". Booking #" << bookings[i].bookingId << "\n";
            cout << "   Flight: " << origin << " to " << destination << "\n";
            cout << "   Travel Date: " << travelDate << "\n";
            cout << "   Seats: " << bookings[i].seatsBooked << " (" << bookings[i].classType << ")\n";
            cout << "   Fare: $" << fixed << setprecision(2) << bookings[i].farePaid << "\n";
            cout << "   Status: " << bookings[i].status << "\n";
            cout << "   ------------------------------\n";
        }
    }
    
    if (count == 0) {
        cout << "No recent bookings found.\n";
    }
}

void generatePersonalReport() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "      PERSONAL BOOKING REPORT\n";
    cout << "========================================\n";
    
    displayPassengerInfo();
    displayBookingSummary();
    displayBookingHistory();
    displayRecentBookings();
    
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    cout << "\nReport Generated: " 
         << formatDate({currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900})
         << " at " << formatTime({currentTime->tm_hour, currentTime->tm_min}) << "\n";
    
    cout << "========================================\n";
    cout << "          END OF REPORT\n";
    cout << "========================================\n";
}

// ========== PROFILE UPDATE FUNCTIONS ==========

void displayCurrentProfile() {
    cout << "\n=== YOUR CURRENT PROFILE ===\n";
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            cout << "1. Name: " << passengers[i].name << "\n";
            cout << "2. Email: " << passengers[i].email << "\n";
            cout << "3. Phone: " << passengers[i].phone << "\n";
            cout << "4. Password: ********\n";
            cout << "------------------------------\n";
            cout << "Total Bookings: " << passengers[i].totalBookings << "\n";
            cout << "Total Spent: $" << fixed << setprecision(2) << passengers[i].totalSpent << "\n";
            break;
        }
    }
}

void updateName() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newName[50];
            cout << "\nCurrent Name: " << passengers[i].name << "\n";
            cout << "Enter new name: ";
            cin.ignore();
            cin.getline(newName, 50);
            
            if (strlen(newName) > 0) {
                strcpy(passengers[i].name, newName);
                cout << "Name updated successfully!\n";
            } else {
                cout << "Name cannot be empty!\n";
            }
            return;
        }
    }
}

void updateEmail() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newEmail[50];
            cout << "\nCurrent Email: " << passengers[i].email << "\n";
            
            while (true) {
                cout << "Enter new email: ";
                cin.ignore();
                cin.getline(newEmail, 50);
                
                if (strlen(newEmail) == 0) {
                    cout << "Email cannot be empty!\n";
                    continue;
                }
                
                if (isValidEmail(newEmail)) {
                    bool emailExists = false;
                    for (int j = 0; j < passengerCount; j++) {
                        if (j != i && strcmp(passengers[j].email, newEmail) == 0) {
                            emailExists = true;
                            break;
                        }
                    }
                    
                    if (emailExists) {
                        cout << "This email is already registered!\n";
                    } else {
                        strcpy(passengers[i].email, newEmail);
                        cout << "Email updated successfully!\n";
                        break;
                    }
                } else {
                    cout << "Invalid email format! Use format: user@domain.com\n";
                }
            }
            return;
        }
    }
}

void updatePhone() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newPhone[15];
            cout << "\nCurrent Phone: " << passengers[i].phone << "\n";
            cout << "Enter new phone number: ";
            cin.ignore();
            cin.getline(newPhone, 15);
            
            if (strlen(newPhone) > 0) {
                strcpy(passengers[i].phone, newPhone);
                cout << "Phone number updated successfully!\n";
            } else {
                cout << "Phone number cannot be empty!\n";
            }
            return;
        }
    }
}

void updatePassword() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char currentPass[30];
            char newPass[30];
            char confirmPass[30];
            
            cout << "\n=== CHANGE PASSWORD ===\n";
            cout << "Enter current password: ";
            cin.ignore();
            cin.getline(currentPass, 30);
            
            if (strcmp(passengers[i].password, currentPass) != 0) {
                cout << "Current password is incorrect!\n";
                return;
            }
            
            while (true) {
                cout << "Enter new password (min 6 characters): ";
                cin.getline(newPass, 30);
                
                if (strlen(newPass) < 6) {
                    cout << "Password must be at least 6 characters!\n";
                    continue;
                }
                
                cout << "Confirm new password: ";
                cin.getline(confirmPass, 30);
                
                if (strcmp(newPass, confirmPass) != 0) {
                    cout << "Passwords do not match!\n";
                } else {
                    strcpy(passengers[i].password, newPass);
                    cout << "Password changed successfully!\n";
                    break;
                }
            }
            return;
        }
    }
}
void updateProfile() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    int choice;
    bool updating = true;
    
    while (updating) {
        cout << "\n========================================\n";
        cout << "          UPDATE PROFILE\n";
        cout << "========================================\n";
        
        displayCurrentProfile();
        
        cout << "\nWhat would you like to update?\n";
        cout << "1. Update Name\n";
        cout << "2. Update Email\n";
        cout << "3. Update Phone Number\n";
        cout << "4. Change Password\n";
        cout << "5. Back to Main Menu\n";
        cout << "----------------------------------------\n";
        cout << "Enter your choice (1-5): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1: updateName(); break;
            case 2: updateEmail(); break;
            case 3: updatePhone(); break;
            case 4: updatePassword(); break;
            case 5: 
                cout << "Returning to main menu...\n";
                updating = false;
                break;
            default:
                cout << "Invalid choice! Please enter 1-5.\n";
        }
        
        if (choice >= 1 && choice <= 4) {
            char more;
            cout << "\nDo you want to update something else? (Y/N): ";
            cin >> more;
            
            if (more != 'Y' && more != 'y') {
                updating = false;
                cout << "Profile update completed.\n";
            }
        }
    }
    savePassengers();
}



// ========== File Handling ==========

void savePassengers() {
        if (passengers == nullptr) {
        cout << "Error: Passenger array not initialized!\n";
        return;
    }
    ofstream out("passengers.txt");
    out << passengerCount << endl;

    for (int i = 0; i < passengerCount; i++) {
        out << passengers[i].id << endl
            << passengers[i].name << endl
            << passengers[i].password << endl
            << passengers[i].email << endl
            << passengers[i].phone << endl
            << passengers[i].totalBookings << endl
            << passengers[i].totalSpent << endl;
    }
    out.close();
}

void loadPassengers() {
    ifstream in("passengers.txt");
    if (!in) return;

    in >> passengerCount;
    in.ignore();

    for (int i = 0; i < passengerCount; i++) {
        in >> passengers[i].id;
        in.ignore();
        in.getline(passengers[i].name, 50);
        in.getline(passengers[i].password, 30);
        in.getline(passengers[i].email, 50);
        in.getline(passengers[i].phone, 15);
        in >> passengers[i].totalBookings;
        in >> passengers[i].totalSpent;
        in.ignore();
    }
    in.close();
}

void saveFlights() {

    if (flights == nullptr) {
        cout << "Error: Flight array not initialized!\n";
        return;
    }

    ofstream out("flights.txt");
    out << flightCount << endl;

    for (int i = 0; i < flightCount; i++) {
        out << flights[i].flightNo << endl
            << flights[i].origin << endl
            << flights[i].destination << endl
            << flights[i].departureDate.day << " "
            << flights[i].departureDate.month << " "
            << flights[i].departureDate.year << endl
            << flights[i].departureTime.hour << " "
            << flights[i].departureTime.minute << endl
            << flights[i].arrivalDate.day << " "
            << flights[i].arrivalDate.month << " "
            << flights[i].arrivalDate.year << endl
            << flights[i].arrivalTime.hour << " "
            << flights[i].arrivalTime.minute << endl
            << flights[i].economySeats << " "
            << flights[i].businessSeats << " "
            << flights[i].firstClassSeats << endl
            << flights[i].economyFare << " "
            << flights[i].businessFare << " "
            << flights[i].firstClassFare << endl
            << flights[i].totalSeats << " "
            << flights[i].availableSeats << endl
            << flights[i].distance << endl
            << flights[i].baseFare << endl
            << flights[i].status << endl
            << flights[i].timesBooked << endl
            << flights[i].totalRevenue << endl;
    }
    out.close();
}

void loadFlights() {
    ifstream in("flights.txt");
    if (!in) return;

    in >> flightCount;
    in.ignore();

    for (int i = 0; i < flightCount; i++) {
        in >> flights[i].flightNo;
        in.ignore();
        in.getline(flights[i].origin, 50);
        in.getline(flights[i].destination, 50);

        in >> flights[i].departureDate.day
           >> flights[i].departureDate.month
           >> flights[i].departureDate.year;

        in >> flights[i].departureTime.hour
           >> flights[i].departureTime.minute;

        in >> flights[i].arrivalDate.day
           >> flights[i].arrivalDate.month
           >> flights[i].arrivalDate.year;

        in >> flights[i].arrivalTime.hour
           >> flights[i].arrivalTime.minute;

        in >> flights[i].economySeats
           >> flights[i].businessSeats
           >> flights[i].firstClassSeats;

        in >> flights[i].economyFare
           >> flights[i].businessFare
           >> flights[i].firstClassFare;

        in >> flights[i].totalSeats
           >> flights[i].availableSeats;

        in >> flights[i].distance;
        in >> flights[i].baseFare;
        in.ignore();
        in.getline(flights[i].status, 20);
        in >> flights[i].timesBooked;
        in >> flights[i].totalRevenue;
        in.ignore();
    }
    in.close();
}

void saveBookings() {

     if (bookings == nullptr) {
        cout << "Error: Booking array not initialized!\n";
        return;
    }

    ofstream out("bookings.txt");
    out << bookingCount << endl;

    for (int i = 0; i < bookingCount; i++) {
        out << bookings[i].bookingId << endl
            << bookings[i].passengerId << endl
            << bookings[i].flightNo << endl
            << bookings[i].bookingDate.day << " "
            << bookings[i].bookingDate.month << " "
            << bookings[i].bookingDate.year << endl
            << bookings[i].travelDate.day << " "
            << bookings[i].travelDate.month << " "
            << bookings[i].travelDate.year << endl
            << bookings[i].seatsBooked << endl
            << bookings[i].classType << endl
            << bookings[i].farePaid << endl
            << bookings[i].status << endl;
    }
    out.close();
}

void loadBookings() {
    ifstream in("bookings.txt");
    if (!in) return;

    in >> bookingCount;
    in.ignore();

    for (int i = 0; i < bookingCount; i++) {
        in >> bookings[i].bookingId;
        in >> bookings[i].passengerId;
        in >> bookings[i].flightNo;

        in >> bookings[i].bookingDate.day
           >> bookings[i].bookingDate.month
           >> bookings[i].bookingDate.year;

        in >> bookings[i].travelDate.day
           >> bookings[i].travelDate.month
           >> bookings[i].travelDate.year;

        in >> bookings[i].seatsBooked;
        in.ignore();
        in.getline(bookings[i].classType, 20);
        in >> bookings[i].farePaid;
        in.ignore();
        in.getline(bookings[i].status, 20);
    }
    in.close();
}

// ========== ADMIN FUNCTIONS ==========

void adminMenu() {
    int choice;
    bool loggedIn = true;
    
    while (loggedIn) {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. Add Flight\n";
        cout << "2. View All Flights\n";
        cout << "3. Update Flight\n";
        cout << "4. Delete Flight\n";
        cout << "5. View All Bookings\n";
        cout << "6. Update Passenger\n";
        cout << "7. Remove Passenger\n";
        cout << "8. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addFlight(flights, flightCount);
                break;
            case 2:
                viewFlights(flights, flightCount);
                break;
            case 3:
                updateFlight(flights, flightCount);
                break;
            case 4:
                deleteFlight(flights, flightCount);
                break;
            case 5:
                viewAllBookings();
                break;
            case 6:
                updatePassengerAdmin();
                break;
            case 7:
                removePassengerAdmin();
                break;
            case 8:
                cout << "Logging out...\n";
                loggedIn = false;
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
}

void adminLoginPanel() {
    cout << "\n=== ADMIN LOGIN ===\n";
    
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;
    
    if (username == "admin@gmail.com" && password == "11223344") 
    {
        cout << "\nLogin successful! Welcome Admin!\n";
     
        adminMenu();
    } else 
    {
        cout << "Access Denied! Invalid credentials.\n";
    }
}

void addFlight(Flight flights[], int &flightCount) {
   if (flightCount >= MAX_FLIGHTS) {
        cout << "Cannot add more flights. Maximum capacity reached.\n";
        return;
    }
    
    int flightNo;
    do {
        cout << "Enter Flight Number (positive integer): ";
        cin >> flightNo;
        if (flightNo <= 0) cout << "Invalid flight number!\n";
    } while (flightNo <= 0);
    
    flights[flightCount].flightNo = flightNo;
    cin.ignore();
    
    cout << "Enter Origin: ";
    cin.getline(flights[flightCount].origin, 50);
    
    cout << "Enter Destination: ";
    cin.getline(flights[flightCount].destination, 50);
    
    int day, month, year;
    do {
        cout << "Enter Departure Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[flightCount].departureDate = {day, month, year};
    
    int hour, minute;
    do {
        cout << "Enter Departure Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].departureTime = {hour, minute};
    
    do {
        cout << "Enter Arrival Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[flightCount].arrivalDate = {day, month, year};
    
    do {
        cout << "Enter Arrival Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].arrivalTime = {hour, minute};
    
    do
     {
        cout << "Enter Economy Seats: ";
        cin >> flights[flightCount].economySeats;
    } while (flights[flightCount].economySeats < 0);
    
    do
    {
        cout << "Enter Business Seats: ";
        cin >> flights[flightCount].businessSeats;
    } while (flights[flightCount].businessSeats < 0);
    
    do 
    {
        cout << "Enter First Class Seats: ";
        cin >> flights[flightCount].firstClassSeats;
    } while (flights[flightCount].firstClassSeats < 0);
    
    do 
    {
        cout << "Enter Economy Fare(per KM): ";
        cin >> flights[flightCount].economyFare;
    } while (flights[flightCount].economyFare < 0);
    
    do 
    {
        cout << "Enter Business Fare(per KM): ";
        cin >> flights[flightCount].businessFare;
    } while (flights[flightCount].businessFare < 0);
    
    do 
    {
        cout << "Enter First Class Fare(per KM): ";
        cin >> flights[flightCount].firstClassFare;
    } while (flights[flightCount].firstClassFare < 0);
    
    flights[flightCount].totalSeats = flights[flightCount].economySeats +
                                      flights[flightCount].businessSeats +
                                      flights[flightCount].firstClassSeats;
    flights[flightCount].availableSeats = flights[flightCount].totalSeats;
    
    do 
    {
        cout << "Enter Distance (positive): ";
        cin >> flights[flightCount].distance;
        if (flights[flightCount].distance < 0) cout << "Invalid distance!\n";
    } while (flights[flightCount].distance < 0);
    
    flights[flightCount].baseFare = flights[flightCount].economyFare;
    strcpy(flights[flightCount].status, "Available");
    flights[flightCount].timesBooked = 0;
    flights[flightCount].totalRevenue = 0.0;
    
    flightCount++;
    cout << "\nFlight added successfully!\n";
    saveFlights();
}

void updateFlight(Flight flights[], int flightCount) {
    if (flightCount == 0) {
        cout << "No flights available to update.\n";
        return;
    }
    
    int flightNo;
    cout << "Enter Flight Number to update: ";
    cin >> flightNo;
    
    int index = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "Flight not found.\n";
        return;
    }
    
    cout << "\nUpdating Flight #" << flights[index].flightNo << ":\n";
    cin.ignore();
    
    cout << "Enter new Origin (current: " << flights[index].origin << "): ";
    cin.getline(flights[index].origin, 50);
    
    cout << "Enter new Destination (current: " << flights[index].destination << "): ";
    cin.getline(flights[index].destination, 50);
    
    int day, month, year;
    do {
        cout << "Enter new Departure Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].departureDate = {day, month, year};
    
    int hour, minute;
    do {
        cout << "Enter new Departure Time (hh mm): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].departureTime = {hour, minute};
    
    do {
        cout << "Enter new Arrival Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].arrivalDate = {day, month, year};
    
    do {
        cout << "Enter new Arrival Time (hh mm): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].arrivalTime = {hour, minute};
    
    cout << "Flight updated successfully!\n";
    saveFlights();
}

void deleteFlight(Flight flights[], int &flightCount) {
    if (flightCount == 0) {
        cout << "No flights available to delete.\n";
        return;
    }
    
    int flightNo;
    cout << "Enter Flight Number to delete: ";
    cin >> flightNo;
    
    int index = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "Flight not found.\n";
        return;
    }
    
    for (int i = index; i < flightCount - 1; i++) {
        flights[i] = flights[i + 1];
    }
    
    flightCount--;
    cout << "Flight #" << flightNo << " deleted successfully!\n";
    saveFlights();
}

void updatePassengerAdmin() {
    int passengerId;
    cout << "\nEnter Passenger ID to update: ";
    cin >> passengerId;

    Passenger* passenger = nullptr;

    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == passengerId) {
            passenger = &passengers[i];
            break;
        }
    }

    if (!passenger) {
        cout << "Passenger not found!\n";
        return;
    }

    cin.ignore();
    cout << "Enter New Name: ";
    cin.getline(passenger->name, 50);

    cout << "Enter New Email: ";
    cin.getline(passenger->email, 50);

    cout << "Enter New Phone: ";
    cin.getline(passenger->phone, 20);

    cout << "\nPassenger information updated successfully.\n";
     savePassengers();
}

void removePassengerAdmin() {
    int passengerId;
    cout << "\nEnter Passenger ID to remove: ";
    cin >> passengerId;

    int index = -1;
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == passengerId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Passenger not found!\n";
        return;
    }

    for (int i = 0; i < bookingCount; ) {
        if (bookings[i].passengerId == passengerId) {
            for (int j = i; j < bookingCount - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            bookingCount--;
        } else {
            i++;
        }
    }

    for (int i = index; i < passengerCount - 1; i++) {
        passengers[i] = passengers[i + 1];
    }
    passengerCount--;

    cout << "\nPassenger and related bookings removed successfully.\n";
    savePassengers();
}
void viewAllBookings() {
    cout << "\n=== ALL BOOKINGS ===\n";
    
    if (bookingCount == 0) {
        cout << "No bookings found.\n";
        return;
    }
    
    cout << left << setw(12) << "Booking ID"
         << setw(15) << "Passenger ID"
         << setw(10) << "Flight #"
         << setw(12) << "Travel Date"
         << setw(10) << "Seats"
         << setw(12) << "Class"
         << setw(12) << "Fare Paid($)"
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        string travelDateStr = to_string(bookings[i].travelDate.day) + "/" +
                              to_string(bookings[i].travelDate.month) + "/" +
                              to_string(bookings[i].travelDate.year);
        
        cout << left << setw(12) << bookings[i].bookingId
             << setw(15) << bookings[i].passengerId
             << setw(10) << bookings[i].flightNo
             << setw(12) << travelDateStr
             << setw(10) << bookings[i].seatsBooked
             << setw(12) << bookings[i].classType
             << setw(12) << fixed << setprecision(2) << bookings[i].farePaid
             << setw(12) << bookings[i].status << "\n";
    }
}


//===============================Menues==========================================

// passenger menue
void showPassengerMenu() {
    int choice;
    bool loggedIn = true;
    
    while (loggedIn) {
        cout << "\n=== PASSENGER MENU ===\n";
        cout << "1. View Available Flights (Summary)\n";
        cout << "2. View Flight Details with Seats\n";
        cout << "3. Book a Flight\n";
        cout << "4. View My Bookings\n";
        cout << "5. Cancel Booking\n";
        cout << "6. View Booking Receipt\n";
        cout << "7. Generate Personal Report\n";
        cout << "8. Update Profile\n";
        cout << "9. Logout\n";
        cout << "\nEnter your choice (1-9): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1:
             viewAvailableFlights();
            break;
            case 2: 
            viewFlightDetailsWithSeats();
             break;
            case 3: 
            bookFlight();
             break;
            case 4:
             displayPassengerBookings();
              break;
            case 5:
             cancelBooking(); 
             break;
            case 6: 
                if (currentPassengerId == -1) {
                    cout << "You must login first!\n";
                } else {
                    int receiptId;
                    cout << "Enter Booking ID for receipt: ";
                    cin >> receiptId;
                    generateBookingReceipt(receiptId);
                }
                break;
            case 7: generatePersonalReport(); break;
            case 8: updateProfile(); break;
            case 9: 
                cout << "Logged out successfully!\n";
                loggedIn = false;
                currentPassengerId = -1;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

// ========== MAIN MENU ==========



void mainMenu() {
    int choice;
    
    do {
        cout << "\n=== AIRLINE RESERVATION SYSTEM ===\n";
        cout << "1. Passenger Registration\n";
        cout << "2. Passenger Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit System\n";
        cout << "\nEnter your choice (1-4): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1: 
            PassengerRegistration(); 
            break;
            case 2:
             passengerLogin(); 
             break;
            case 3:
             adminLoginPanel();
              break;
            case 4:
             cout << "Thank you for using the system!\n"; 
             break;
            default:
             cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 4);
}

// ========== MAIN FUNCTION ==========
int main() 
{
    passengers = new Passenger[MAX_PASSENGERS];
    flights = new Flight[MAX_FLIGHTS];
    bookings = new Booking[MAX_BOOKINGS];

    loadPassengers();
    loadFlights();
    loadBookings();
    
    mainMenu();
    
    delete[] passengers;
    delete[] flights;
    delete[] bookings;

    passengers = nullptr;
    flights = nullptr;
    bookings = nullptr;
    
    return 0;
}