#include<iostream>
#include<string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <ctime>
using namespace std;

const int MAX_PASSENGERS = 100;
const int MAX_FLIGHTS = 100;
const int MAX_BOOKINGS = 100;
const int MAX_ADMINS = 5;

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

struct Flight
{
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

struct Admin {
    int id;
    char username[30];
    char password[30];
    char email[50];
};

Passenger passengers[MAX_PASSENGERS];
Flight flights[MAX_FLIGHTS];
Booking bookings[MAX_BOOKINGS];
Admin admins[MAX_ADMINS];

int passengerCount = 0;
int flightCount = 0;
int bookingCount = 0;
int adminCount = 0;
int currentPassengerId = -1;
int currentAdminId = -1;

// Function prototypes
void viewAvailableFlights();
void bookFlight();
void showPassengerMenu();
void passengerLogin();
bool isValidEmail(const char* email);
void PassengerRegistration();
void mainMenu();
bool isValidDate(const Date& date);
bool isFutureDate(const Date& date);
float calculateFare(const Flight& flight, int seats, const string& classType);
int generateBookingId();
void addSampleData();

Booking* findBookingById(int bookingId, int& index);
Flight* findFlightByNumber(int flightNo, int& index);
float calculateRefundAmount(const Booking& booking);
void displayPassengerBookings();
void cancelBooking();

// Validation of date

bool isValidDate(const Date& date) {
    if (date.day < 1 || date.day > 31){
        return false;
    }
     
    if (date.month < 1 || date.month > 12){
     return false;
    }
    if (date.year < 2024) {
         return false;
    }
   
    return true;
}
// Ensure date of booking is in future

bool isFutureDate(const Date& date) {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    
    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;
    
    if (date.year < currentYear) {
        return false;
    }
    
    if (date.year == currentYear && date.month < currentMonth){
         return false;
    }
    
    if (date.year == currentYear && date.month == currentMonth && date.day <= currentDay){
        return false;
    } 
    
    return true;
}
// Calculate Total Fare

float calculateFare(const Flight& flight, int seats, const string& classType) {
    float multiplier;
    
    if (classType == "Economy"){
        multiplier = 1.0;
    } 
    else if (classType == "Business"){
        multiplier = 2.0;
    } 
    else if (classType == "First"){
        multiplier = 3.5;
    } 
    else{
        multiplier = 1.0;
    } 
    
    return flight.baseFare * seats * multiplier;
}
//Book Flight

int generateBookingId() {
    //Static ID 
    static int lastId = 1000;
    return ++lastId;
}
//Validation of Email

bool isValidEmail(const char* email) {
    int atCount = 0;
    int dotCount = 0;
    int len = strlen(email);
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@')
         atCount++;
        if (email[i] == '.')
         dotCount++;
    }
    
    return (atCount == 1 && dotCount >= 1 && len >= 5);
}

// ========== VIEW AVAILABLE FLIGHTS ==========

void viewAvailableFlights() {
    cout << "\n=== AVAILABLE FLIGHTS ===\n";
    
    if (flightCount == 0) {
        cout << "Sorry! No flights available at the moment.\n";
        return;
    }
    
    cout << left << setw(10) << "Flight #" 
         << setw(15) << "From" 
         << setw(15) << "To" 
         << setw(10) << "Seats" 
         << setw(10) << "Fare" 
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].status, "Available") == 0 && flights[i].availableSeats > 0) {
          
            cout << left << setw(10) << flights[i].flightNo
                 << setw(15) << flights[i].origin
                 << setw(15) << flights[i].destination
                 << setw(10) << flights[i].availableSeats
                 << setw(10) << "$" << flights[i].baseFare
                 << setw(12) << flights[i].status << "\n";
        }
    }
}

//  BOOK FLIGHT 
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
    
    int flightChoice;
    cout << "\nEnter Flight Number to book (0 to cancel): ";
    cin >> flightChoice;
    
    if (flightChoice == 0)
     return;
    
    Flight* selectedFlight = nullptr;
    int flightIndex = -1;
    
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightChoice && 
            strcmp(flights[i].status, "Available") == 0 && 
            flights[i].availableSeats > 0) {
            selectedFlight = &flights[i];
            flightIndex = i;
            break;
        }
    }
    
    if (!selectedFlight) {
        cout << "Invalid flight selection or flight not available!\n";
        return;
    }
    
    // Get number of seats

    int seats;
    cout << "Number of seats to book (1-" << selectedFlight->availableSeats << "): ";
    cin >> seats;
    
    while (seats < 1 || seats > selectedFlight->availableSeats) {
        cout << "Invalid! Enter between 1 and " << selectedFlight->availableSeats << " seats: ";
        cin >> seats;
    }
    
    // Get class type
    cout << "\nSelect Class Type:\n";
    cout << "1. Economy (1.0x)\n";
    cout << "2. Business (2.0x)\n";
    cout << "3. First Class (3.5x)\n";
    
    int classChoice;
    cout << "Enter choice (1-3): ";
    cin >> classChoice;
    
    string classType;
    switch(classChoice) {
        case 1:
        {
            classType = "Economy"; 
            break;
        } 
        case 2:
        {
            classType = "Business";
             break;
        } 
        case 3:
        {
            classType = "First"; 
            break;
        } 
        default: 
            cout << "Invalid choice! Defaulting to Economy.\n";
            classType = "Economy";
            break;
    }
    
    // Get travel date
    
    Date travelDate;
    cout << "\nEnter travel date (DD MM YYYY): ";
    cin >> travelDate.day >> travelDate.month >> travelDate.year;
    
    // Validate date
    if (!isValidDate(travelDate)) {
        cout << "Invalid date!\n";
        return;
    }
    
    if (!isFutureDate(travelDate)) {
        cout << "Travel date must be in the future!\n";
        return;
    }
    
    // Calculate fare
    float fare = calculateFare(*selectedFlight, seats, classType);
    
    // Show booking summary
    cout << "\n=== BOOKING SUMMARY ===\n";
    cout << "Flight: " << selectedFlight->origin << " to " << selectedFlight->destination << "\n";
    cout << "Date: " << travelDate.day << "/" << travelDate.month << "/" << travelDate.year << "\n";
    cout << "Seats: " << seats << " (" << classType << " class)\n";
    cout << "Total Fare: $" << fixed << setprecision(2) << fare << "\n";
    
    char confirm;
    cout << "\nConfirm booking? (Y/N): ";
    cin >> confirm;
    
    // if (toupper(confirm) != 'Y') {
    //     cout << "Booking cancelled.\n";
    //     return;
    // }

     if (confirm != 'Y') {
        cout << "Booking cancelled.\n";
        return;
    }
    
    // Create booking

    Booking newBooking;
    newBooking.bookingId = generateBookingId();
    newBooking.passengerId = currentPassengerId;
    newBooking.flightNo = flightChoice;
    
    // Set current date as booking date
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
    
    // Add booking to array
    bookings[bookingCount++] = newBooking;
    
    // Update flight
    flights[flightIndex].availableSeats -= seats;   // Minus seat
    flights[flightIndex].timesBooked++;
    flights[flightIndex].totalRevenue += fare;  //Calculate revnue
     //If flight is full
    if (flights[flightIndex].availableSeats == 0) {
        strcpy(flights[flightIndex].status, "Full");
    }
    
    // Update passenger
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalBookings++;
            passengers[i].totalSpent += fare;
            break;
        }
    }
    
    cout << "\nBooking confirmed! Booking ID: " << newBooking.bookingId << "\n";
    cout << "Receipt generated successfully!\n";
}

// ========== PASSENGER MENU ==========
void showPassengerMenu() {
    int choice;
    bool loggedIn = true;
    
    while (loggedIn) {
        cout << "\n=== PASSENGER MENU ===\n";
        cout << "1. View Available Flights\n";
        cout << "2. Book a Flight\n";
        cout << "3. Cancel Booking\n";
        cout << "4. Generate Personal Report\n";
        cout << "5. Update Profile\n";
        cout << "6. Logout\n";
        cout << "\nEnter your choice (1-6): ";
        
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
            {
                viewAvailableFlights(); 
                break;
            }  
            case 2:
            { 
                bookFlight(); 
                break;
            }
            case 3:
            {
                 cancelBooking();
                break;
            } 
            case 4: 
                {
                    cout << "Generate Report feature coming soon!\n";
                break;
                }
            case 5: 
               { cout << "Update Profile feature coming soon!\n";
                break;
                }
            case 6: 
               {
                 cout << "Logged out successfully!\n";
                loggedIn = false;
                currentPassengerId = -1;
                break;
                }
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

// ========== PASSENGER LOGIN ==========
void passengerLogin() {
    cout << "\n=== PASSENGER LOGIN ===\n";
    
    int id;
    char password[30];
    
    cout << "Enter Passenger ID: ";
    cin >> id;
    cin.ignore();
    
    cout << "Enter Password: ";
    cin.getline(password, 30);
    cout<<endl;
    bool login = false;

    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == id && strcmp(passengers[i].password, password) == 0) {
            currentPassengerId = id;
            cout << "\nLogin successful! Welcome " << passengers[i].name << "!\n";
            login = true;
            showPassengerMenu();
            return;
        }
    }
    
    if (!login) {
        cout << "Invalid credentials! Please try again.\n";
    }
}

// ========== PASSENGER REGISTRATION ==========
    void PassengerRegistration() {
    if (passengerCount >= MAX_PASSENGERS) {
        cout << "Maximum passenger limit reached! Cannot register more passengers.\n";
        return;
    }

    Passenger newPassenger;
    
    cout << "\n=== PASSENGER REGISTRATION ===\n";
    
    // Auto-generate ID
    newPassenger.id = 1000 + passengerCount + 1;
    cout << "Your Passenger ID: " << newPassenger.id << " (Remember this for login)\n";
    
    // Get name
    cout << "Enter your name: ";
    cin.ignore();
    cin.getline(newPassenger.name, 50);
    
    // Get password
    cout << "Enter your Password: ";
    cin.getline(newPassenger.password, 30);
    
    // Get email with validation
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
    
    // Get phone
    cout << "Enter Phone Number: ";
    cin.getline(newPassenger.phone, 15);
    
    // Initialize other fields
    newPassenger.totalBookings = 0;
    newPassenger.totalSpent = 0.0;
    
    // Add to array
    passengers[passengerCount] = newPassenger;
    passengerCount++;
    
    cout << "\nRegistration successful!\n";


    cout << "\n=== REGISTRATION DETAILS ===\n";
    cout << "Passenger ID: " << newPassenger.id << "\n";
    cout << "Name: " << newPassenger.name << "\n";
    cout << "Email: " << newPassenger.email << "\n";
    cout << "Phone: " << newPassenger.phone << "\n";
    cout << "\nIMPORTANT: Save your Passenger ID for login: " << newPassenger.id << "\n";
}
// ========Cancel Booking Feature ========


// Function to find booking by ID
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

// Function to find flight by flight number

Flight* findFlightByNumber(int flightNo, int& index) {
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            return &flights[i];
        }
    }
    return nullptr;
}

// Function to calculate refund amount
float calculateRefundAmount(const Booking& booking) {
    // Get current date
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    Date currentDate = {
                     currentTime->tm_mday, 
                     currentTime->tm_mon + 1, 
                     currentTime->tm_year + 1900
                    };
    
    // Calculate days before travel
    int daysBefore = 0;
    
    // Simple calculation: assume 30 days per month
    daysBefore = (booking.travelDate.year - currentDate.year) * 365 +
                 (booking.travelDate.month - currentDate.month) * 30 +
                 (booking.travelDate.day - currentDate.day);
    
    /*This refund Refund policy applied
    - 7+ days before: 90% refund
    - 3-6 days before: 50% refund  
    - 1-2 days before: 20% refund
    - Same day or past: No refund*/
    
    if (daysBefore >= 7) {
        return booking.farePaid * 0.9;  // 90% refund
    } else if (daysBefore >= 3) {
        return booking.farePaid * 0.5;  // 50% refund
    } else if (daysBefore >= 1) {
        return booking.farePaid * 0.2;  // 20% refund
    } else {
        return 0.0;  // No refund
    }
}

// Function to display passenger's bookings
void displayPassengerBookings() {
    cout << "\n===== YOUR BOOKINGS =====\n";
    
    bool found = false;
    
    cout << left << setw(12) << "Booking ID"
         << setw(10) << "Flight #  "
         << setw(15) << "Travel Date"
         << setw(10) << "Seats"
         << setw(12) << "Class"
         << setw(12) << "Fare Paid($)"
         << setw(12) << "Status" << "\n";
    cout << string(85, '-') << "\n";   // sufyan ye 85 times ---- print kry gi
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            found = true;
            
            // Format travel date
            string travelDateStr = to_string(bookings[i].travelDate.day) + "/" +
                                  to_string(bookings[i].travelDate.month) + "/" +
                                  to_string(bookings[i].travelDate.year);
            
            cout << left << setw(12) << bookings[i].bookingId
                 << setw(10) << bookings[i].flightNo
                 << setw(15) << travelDateStr
                 << setw(10) << bookings[i].seatsBooked
                 << setw(12) << bookings[i].classType
                 << setw(12) << fixed << setprecision(2) << bookings[i].farePaid
                 << setw(12) << bookings[i].status << "\n";
        }
    }
    
    if (!found) {
        cout << "No bookings found.\n";
    }
}

// Main cancel booking function
void cancelBooking() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n=== CANCEL BOOKING ===\n";
    
    // Show passenger's bookings
    displayPassengerBookings();
    
    // Check if passenger has any bookings
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
    
    // Get booking ID to cancel
    int bookingId;
    cout << "\nEnter Booking ID to cancel (0 to go back): ";
    cin >> bookingId;
    
    if (bookingId == 0) {
        return;
    }
    
    // Find the booking
    int bookingIndex = -1;
    Booking* bookingToCancel = findBookingById(bookingId, bookingIndex);
    
    if (!bookingToCancel) {
        cout << "Invalid Booking ID or booking not found!\n";
        return;
    }
    
    // Check if booking is already cancelled
    if (strcmp(bookingToCancel->status, "Cancelled") == 0) {
        cout << "This booking is already cancelled.\n";
        return;
    }
    
    // Check if booking is completed
    if (strcmp(bookingToCancel->status, "Completed") == 0) {
        cout << "Cannot cancel a completed booking.\n";
        return;
    }
    
    // Calculate refund
    float refundAmount = calculateRefundAmount(*bookingToCancel);
    
    // Show cancellation details
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
    
    // Ask for confirmation
    char confirm;
    cout << "\nAre you sure you want to cancel this booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Cancellation cancelled.\n";
        return;
    }
    
    // Find the flight to update seats
    int flightIndex = -1;
    Flight* flight = findFlightByNumber(bookingToCancel->flightNo, flightIndex);
    
    if (flight) {
        // Update flight seats
        flight->availableSeats += bookingToCancel->seatsBooked;
        
        // Update flight status if it was Full
        if (strcmp(flight->status, "Full") == 0) {
            strcpy(flight->status, "Available");
        }
        
        // Update flight revenue
        flight->totalRevenue -= refundAmount;
        
        // Update flight booking count
        flight->timesBooked--;
    }
    
    // Update booking status
    strcpy(bookingToCancel->status, "Cancelled");
    
    // Update passenger's total spent
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalSpent -= refundAmount;
            passengers[i].totalBookings--; // Decrement booking count
            break;
        }
    }
    
    cout << "\n=== CANCELLATION SUCCESSFUL ===\n";
    cout << "Booking ID " << bookingToCancel->bookingId << " has been cancelled.\n";
    
    if (refundAmount > 0) {
        cout << "Refund of $" << fixed << setprecision(2) << refundAmount 
             << " will be processed to your account.\n";
    }
    
    // Show remaining balance if any
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            if (passengers[i].totalSpent > 0) {
                cout << "Your remaining total spent: $" 
                     << fixed << setprecision(2) << passengers[i].totalSpent << "\n";
            }
            break;
        }
    }
}
bool isValidDate(int day, int month, int year)
{
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

bool isValidTime(int hour, int minute)
{
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

//===============View Booking===================

void viewBookings(Flight flights[], int flightCount)
{
    if (flightCount == 0)
    {
        cout << "No flights available.\n";
        return;
    }

    cout << "\n========== FLIGHT BOOKINGS ==========\n";

    for (int i = 0; i < flightCount; i++)
    {
        cout << "\n--------------------------------------\n";
        cout << "Flight Number   : " << flights[i].flightNo << endl;
        cout << "Origin          : " << flights[i].origin << endl;
        cout << "Destination     : " << flights[i].destination << endl;

        cout << "Economy Seats Remaining   : " << flights[i].economySeats << endl;
        cout << "Business Seats Remaining  : " << flights[i].businessSeats << endl;
        cout << "First Class Seats Remaining: " << flights[i].firstClassSeats << endl;

        cout << "Times Booked    : " << flights[i].timesBooked << endl;
        cout << "Total Revenue   : " << flights[i].totalRevenue << endl;
    }

    cout << "\n======================================\n";
}

//=============== Flight Delete===================

void deleteFlight(Flight flights[], int &flightCount)
{
    if (flightCount == 0)
    {
        cout << "No flights available to delete.\n";
        return;
    }

    int flightNo;
    cout << "Enter Flight Number to delete: ";
    cin >> flightNo;

    // Find the flight index
    int index = -1;
    for (int i = 0; i < flightCount; i++)
    {
        if (flights[i].flightNo == flightNo)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Flight not found.\n";
        return;
    }

    // Shift all flights after the deleted one
    for (int i = index; i < flightCount - 1; i++)
    {
        flights[i] = flights[i + 1];
    }

    flightCount--; // Reduce total flight count
    cout << "Flight #" << flightNo << " deleted successfully!\n";
}

//=============== Flight Update===================

void updateFlight(Flight flights[], int flightCount)
{
    if (flightCount == 0)
    {
        cout << "No flights available to update.\n";
        return;
    }

    int flightNo;
    cout << "Enter Flight Number to update: ";
    cin >> flightNo;
    cin.ignore();

    int index = -1;
    for (int i = 0; i < flightCount; i++)
    {
        if (flights[i].flightNo == flightNo)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Flight not found.\n";
        return;
    }

    cout << "\nUpdating Flight #" << flights[index].flightNo << ":\n";

    cout << "Enter new Origin (current: " << flights[index].origin << "): ";
    cin.ignore();
    cin.getline(flights[index].origin, 50);

    cout << "Enter new Destination (current: " << flights[index].destination << "): ";
    cin.getline(flights[index].destination, 50);

    int day, month, year;
    do {
        cout << "Enter new Departure Date (dd mm yyyy) (current: "
             << flights[index].departureDate.day << "/"
             << flights[index].departureDate.month << "/"
             << flights[index].departureDate.year << "): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].departureDate = {day, month, year};

    int hour, minute;
    do {
        cout << "Enter new Departure Time (hh mm, 0-23 & 0-59) (current: "
             << flights[index].departureTime.hour << ":"
             << flights[index].departureTime.minute << "): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].departureTime = {hour, minute};

    do {
        cout << "Enter new Arrival Date (dd mm yyyy) (current: "
             << flights[index].arrivalDate.day << "/"
             << flights[index].arrivalDate.month << "/"
             << flights[index].arrivalDate.year << "): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].arrivalDate = {day, month, year};

    do {
        cout << "Enter new Arrival Time (hh mm, 0-23 & 0-59) (current: "
             << flights[index].arrivalTime.hour << ":"
             << flights[index].arrivalTime.minute << "): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].arrivalTime = {hour, minute};

    do {
        cout << "Enter Economy Seats (current: " << flights[index].economySeats << "): ";
        cin >> flights[index].economySeats;
    } while (flights[index].economySeats < 0);

    do {
        cout << "Enter Business Seats (current: " << flights[index].businessSeats << "): ";
        cin >> flights[index].businessSeats;
    } while (flights[index].businessSeats < 0);

    do {
        cout << "Enter First Class Seats (current: " << flights[index].firstClassSeats << "): ";
        cin >> flights[index].firstClassSeats;
    } while (flights[index].firstClassSeats < 0);

    flights[index].totalSeats = flights[index].economySeats + flights[index].businessSeats + flights[index].firstClassSeats;
    flights[index].availableSeats = flights[index].totalSeats - flights[index].timesBooked;

    do {
        cout << "Enter Economy Fare (current: " << flights[index].economyFare << "): ";
        cin >> flights[index].economyFare;
    } while (flights[index].economyFare < 0);

    do {
        cout << "Enter Business Fare (current: " << flights[index].businessFare << "): ";
        cin >> flights[index].businessFare;
    } while (flights[index].businessFare < 0);

    do {
        cout << "Enter First Class Fare (current: " << flights[index].firstClassFare << "): ";
        cin >> flights[index].firstClassFare;
    } while (flights[index].firstClassFare < 0);

    do {
        cout << "Enter Distance (current: " << flights[index].distance << "): ";
        cin >> flights[index].distance;
        if (flights[index].distance < 0) cout << "Distance cannot be negative!\n";
    } while (flights[index].distance < 0);

    if (flights[index].availableSeats == 0)
        strcpy(flights[index].status, "Full");
    else
        strcpy(flights[index].status, "Available");

    cout << "\nFlight updated successfully!\n";
}

// ========== Flight View==========

void viewFlights(Flight flights[], int flightCount)
{
    if (flightCount == 0)
    {
        cout << "\nNo flights available.\n";
        return;
    }

    cout << "\n========== AVAILABLE FLIGHTS ==========\n";

    for (int i = 0; i < flightCount; i++)
    {
        cout << "\n--------------------------------------\n";
        cout << "Flight Number   : " << flights[i].flightNo << endl;
        cout << "Origin          : " << flights[i].origin << endl;
        cout << "Destination     : " << flights[i].destination << endl;

        cout << "Departure Date  : "
             << flights[i].departureDate.day << "/"
             << flights[i].departureDate.month << "/"
             << flights[i].departureDate.year << endl;

        cout << "Departure Time  : "
             << flights[i].departureTime.hour << ":"
             << flights[i].departureTime.minute << endl;

        cout << "Arrival Date    : "
             << flights[i].arrivalDate.day << "/"
             << flights[i].arrivalDate.month << "/"
             << flights[i].arrivalDate.year << endl;

        cout << "Arrival Time    : "
             << flights[i].arrivalTime.hour << ":"
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

//=============== Flight Added===================

void addFlight(Flight flights[], int &flightCount)
{
    if (flightCount >= 100)
    {
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

    // Departure Time
    int hour, minute;
    do {
        cout << "Enter Departure Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].departureTime = {hour, minute};

    // Arrival Date
    do {
        cout << "Enter Arrival Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[flightCount].arrivalDate = {day, month, year};

    // Arrival Time
    do {
        cout << "Enter Arrival Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].arrivalTime = {hour, minute};

    // Seats
    do {
        cout << "Enter Economy Seats: ";
        cin >> flights[flightCount].economySeats;
    } while (flights[flightCount].economySeats < 0);

    do {
        cout << "Enter Business Seats: ";
        cin >> flights[flightCount].businessSeats;
    } while (flights[flightCount].businessSeats < 0);

    do {
        cout << "Enter First Class Seats: ";
        cin >> flights[flightCount].firstClassSeats;
    } while (flights[flightCount].firstClassSeats < 0);

    // Fares
    do {
        cout << "Enter Economy Fare: ";
        cin >> flights[flightCount].economyFare;
    } while (flights[flightCount].economyFare < 0);

    do {
        cout << "Enter Business Fare: ";
        cin >> flights[flightCount].businessFare;
    } while (flights[flightCount].businessFare < 0);

    do {
        cout << "Enter First Class Fare: ";
        cin >> flights[flightCount].firstClassFare;
    } while (flights[flightCount].firstClassFare < 0);

    // Total & available seats
    flights[flightCount].totalSeats =
        flights[flightCount].economySeats +
        flights[flightCount].businessSeats +
        flights[flightCount].firstClassSeats;
    flights[flightCount].availableSeats = flights[flightCount].totalSeats;

    // Distance
    do {
        cout << "Enter Distance (positive): ";
        cin >> flights[flightCount].distance;
        if (flights[flightCount].distance < 0) cout << "Invalid distance!\n";
    } while (flights[flightCount].distance < 0);

    strcpy(flights[flightCount].status, "Available");
    flights[flightCount].timesBooked = 0;
    flights[flightCount].totalRevenue = 0.0;

    flightCount++;

    cout << "\nFlight added successfully with class-wise fares!\n";
}



// ========== Admin Menu ==========
void adminMenu()
{
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. Add Flight\n";
        cout << "2. View Flights\n";
        cout << "3. Update Flight\n";
        cout << "4. Delete Flight\n";
        cout << "5. View Bookings\n";
        cout << "6. Logout\n";
 }

 // ========== Admin Login ==========

        void adminLoginPanel() {
        cout << "\n=== ADMIN LOGIN ===\n";

        string username;
        string password;

        cout << "Enter Admin Username: ";
        cin >> username;

        cout << "Enter Admin Password: ";
        cin >> password;

        if (username == "admin" && password == "1234")
        {
        cout << "\nLogin successful! Welcome Admin!\n";
        int choice;
        Flight flights[100];  // array to store flights
        int flightCount = 0;
        do {
        adminMenu();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
        {
        addFlight(flights,flightCount);
        }
        break;

        case 2:
        {
        viewFlights(flights,flightCount);
        }
        break;

        case 3:
        {
        updateFlight(flights,flightCount);
        }
        break;

        case 4:
        {
        deleteFlight(flights,flightCount);
        }
        break;

        case 5:
        {
        viewBookings(flights,flightCount);
        }
        break;

        case 6:
        {
        cout << "Logging out...\n";
        }
        break;

        default:
        {
        cout << "Invalid choice!\n";
        }
        }
        } while (choice != 6);
        }
        else
        {
        cout << "Access Denied! Invalid credentials.\n";
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
        cin.ignore();

        switch(choice) {
            case 1:
            {
                PassengerRegistration();
                break;
            }
            case 2:
               { 
                passengerLogin();
                break;
                 }
            case 3:
                {
                adminLoginPanel();
                break;
                }
            case 4:
                {
                    cout << "Thank you for using the system!\n";
                break;
                }
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 4);
}

// ========== MAIN FUNCTION ==========

int main() {
    // Add one sample passenger for testing
    // passengerCount = 1;
    // passengers[0].id = 1001;
    // strcpy(passengers[0].name, "Test User");
    // strcpy(passengers[0].password, "test123");
    // strcpy(passengers[0].email, "test@example.com");
    // strcpy(passengers[0].phone, "1234567890");
    // passengers[0].totalBookings = 0;
    // passengers[0].totalSpent = 0.0;
    
    // Add one sample flight for testing
    flightCount = 1;
    flights[0].flightNo = 101;
    strcpy(flights[0].destination, "Pakistan");
    strcpy(flights[0].origin, "India");
    flights[0].departureDate = {15, 12, 2024};
    flights[0].departureTime = {8, 0};
    flights[0].arrivalDate = {15, 12, 2024};
    flights[0].arrivalTime = {16, 30};
    flights[0].totalSeats = 200;
    flights[0].availableSeats = 180;
    flights[0].baseFare = 300.0;
    flights[0].distance = 4000.0;
    strcpy(flights[0].status, "Available");
    flights[0].timesBooked = 20;
    flights[0].totalRevenue = 6000.00;
    
    mainMenu();
    return 0;
}