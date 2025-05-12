#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <mysql.h>
#include <ctime>
#include <sstream>

using namespace std;

MYSQL *conn;

string to_string(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

void connectDatabase()
{
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "1234", "service_app", 3306, NULL, 0);
    if (conn)
    {
        cout << "Database Connected Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Connect!" << endl;
    }
}

int generateID()
{
    srand(time(0));
    return rand() % 9000 + 1000;
}

class Customer
{
public:
    void signUp();
    void logIn();
    void requestService(int id);
    void editAccount(int id);
    void deleteAccount(int id);
    void viewAssignedTasks(int id);
    void chatWithWorker(int id);
    void viewMessages(int id);
    void giveReview(int id);
    void loggedInCustomer(int id);
    void searchWorkers();
};

class Worker
{
public:
    void signUp();
    void logIn();
    void viewRequests(int id);
    void respondToRequest(int id);
    void editAccount(int id);
    void deleteAccount(int id);
    void viewAssignedTasks(int id);
    void chatWithCustomer(int id);
    void viewMessages(int id);
    void loggedInWorker(int id);
};

void Customer::signUp()
{
    int id = generateID();
    string name, phone, city, password;
    cout << "Enter Name: ";
    cin >> name;
    cout << "Enter Phone: ";
    cin >> phone;
    cout << "Enter City: ";
    cin >> city;
    cout << "Enter Password: ";
    cin >> password;
    string query = "INSERT INTO customers (customer_id, name, phone, city, password) VALUES (" + to_string(id) + ", '" + name + "', '" + phone + "', '" + city + "', '" + password + "')";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Sign Up Successful! Your ID is: " << id << endl;
    }
    else
    {
        cout << "Sign Up Failed!" << endl;
    }
}

void Customer::logIn()
{
    int id;
    string password;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;
    string query = "SELECT * FROM customers WHERE customer_id = " + to_string(id) + " AND password = '" + password + "'";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0)
    {
        cout << "Login Successful!" << endl;
        loggedInCustomer(id);
    }
    else
    {
        cout << "Login Failed!" << endl;
    }
}

void Customer::loggedInCustomer(int id)
{
    int loggedInCustomerChoice;
    do
    {
        cout << "\n========= Logged In Customer Menu =========" << endl;
        cout << "1. Request Service" << endl;
        cout << "2. Edit Account" << endl;
        cout << "3. Delete Account" << endl;
        cout << "4. View Assigned Tasks" << endl;
        cout << "5. Chat with Worker" << endl;
        cout << "6. Give Review" << endl;
        cout << "7. View Messages" << endl;
        cout << "8. Search Workers" << endl; // Add this option
        cout << "0. Log Out" << endl;
        cout << "Enter Your Choice: ";
        cin >> loggedInCustomerChoice;

        if (loggedInCustomerChoice == 1)
        {
            requestService(id);
        }
        else if (loggedInCustomerChoice == 2)
        {
            editAccount(id);
        }
        else if (loggedInCustomerChoice == 3)
        {
            deleteAccount(id);
        }
        else if (loggedInCustomerChoice == 4)
        {
            viewAssignedTasks(id);
        }
        else if (loggedInCustomerChoice == 5)
        {
            chatWithWorker(id);
        }
        else if (loggedInCustomerChoice == 6)
        {
            giveReview(id);
        }
        else if (loggedInCustomerChoice == 7)
        {
            viewMessages(id);
        }
        else if (loggedInCustomerChoice == 8)
        {
            searchWorkers();
        }
    } while (loggedInCustomerChoice != 0);
}

void Customer::searchWorkers()
{
    string queryStr;
    cout << "Enter any of details separated by commas (id, name, city, category): ";
    cin.ignore();
    getline(cin, queryStr);

    stringstream ss(queryStr);
    string criterion;
    string query = "SELECT * FROM workers WHERE ";
    bool firstCriterium = true;
    while (getline(ss, criterion, ','))
    {
        criterion.erase(0, criterion.find_first_not_of(" \t"));
        criterion.erase(criterion.find_last_not_of(" \t") + 1);

        if (!firstCriterium)
        {
            query += " AND ";
        }
        firstCriterium = false;

        query += "(worker_id LIKE '%" + criterion + "%' OR name LIKE '%" + criterion + "%' OR city LIKE '%" + criterion + "%' OR work_category LIKE '%" + criterion + "%')";
    }

    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if (mysql_num_rows(res) > 0)
        {
            cout << "Search results:" << endl;
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Worker ID: " << row[0] << ", Name: " << row[1] << ", City: " << row[3] << ", Category: " << row[5] << endl;
            }
        }
        else
        {
            cout << "No workers found." << endl;
        }
    }
    else
    {
        cout << "Failed to search workers." << endl;
    }
}

void Customer::requestService(int id)
{
    string description, city, category;
    double proposed_fee;
    cout << "Enter City: ";
    cin >> city;
    cout << "Enter Category: ";
    cin >> category;
    cout << "Enter Description: ";
    cin.ignore();
    getline(cin, description);
    cout << "Enter Proposed Fee: ";
    cin >> proposed_fee;
    string query = "INSERT INTO service_requests (customer_id, description, city, category, proposed_fee) VALUES (" + to_string(id) + ", '" + description + "', '" + city + "', '" + category + "', " + to_string(proposed_fee) + ")";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Service Requested Successfully!" << endl;
    }
    else
    {
        cout << "Service Request Failed!" << endl;
    }
}

void Customer::editAccount(int id)
{
    string name, phone, city, password;
    cout << "Enter New Name: ";
    cin >> name;
    cout << "Enter New Phone: ";
    cin >> phone;
    cout << "Enter New City: ";
    cin >> city;
    cout << "Enter New Password: ";
    cin >> password;
    string query = "UPDATE customers SET name = '" + name + "', phone = '" + phone + "', city = '" + city + "', password = '" + password + "' WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Account Updated Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Update Account!" << endl;
    }
}

void Customer::deleteAccount(int id)
{
    string query = "DELETE FROM customers WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Account Deleted Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Delete Account!" << endl;
    }
}

void Customer::viewAssignedTasks(int id)
{
    string query = "SELECT * FROM service_requests WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            cout << "Request ID: " << row[0] << ", Description: " << row[3] << ", Fee: " << row[4] << endl;
        }
    }
    else
    {
        cout << "Unable to fetch assigned tasks." << endl;
    }
}

void Customer::chatWithWorker(int id)
{
    string query = "SELECT request_id, worker_id FROM service_requests WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if (mysql_num_rows(res) > 0)
        {
            cout << "Select a request to chat with the worker:" << endl;
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Request ID: " << row[0] << ", Worker ID: " << row[1] << endl;
            }
            int request_id;
            cout << "Enter Request ID: ";
            cin >> request_id;
            string message;
            cout << "Enter Message: ";
            cin.ignore();
            getline(cin, message);
            query = "INSERT INTO chats (request_id, sender_role, message) VALUES (" + to_string(request_id) + ", 'customer', '" + message + "')";
            q = query.c_str();
            qstate = mysql_query(conn, q);
            if (!qstate)
            {
                cout << "Message Sent to Worker!" << endl;
            }
            else
            {
                cout << "Failed to Send Message!" << endl;
                cout << "MySQL Error: " << mysql_error(conn) << endl;
            }
        }
        else
        {
            cout << "No requests found." << endl;
        }
    }
    else
    {
        cout << "Failed to fetch requests." << endl;
    }
}
void Customer::viewMessages(int id)
{
    string query = "SELECT c.request_id, c.message, c.timestamp, c.sender_role FROM chats c JOIN service_requests sr ON c.request_id = sr.request_id WHERE sr.customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if (mysql_num_rows(res) > 0)
        {
            cout << "Messages:" << endl;
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Request ID: " << row[0] << ", Message: " << row[1] << ", Timestamp: " << row[2] << ", Sender: " << row[3] << endl;
            }
        }
        else
        {
            cout << "No messages found." << endl;
        }
    }
    else
    {
        cout << "Failed to Fetch Messages!" << endl;
        cout << "MySQL Error: " << mysql_error(conn) << endl;
    }
}

void Customer::giveReview(int id)
{
    int worker_id;
    int rating;
    string comment;
    cout << "Enter Worker ID: ";
    cin >> worker_id;
    cout << "Enter Rating (1-5): ";
    cin >> rating;
    cout << "Enter Comment: ";
    cin.ignore();
    getline(cin, comment);
    string query = "INSERT INTO reviews (worker_id, customer_id, rating, comment) VALUES (" + to_string(worker_id) + ", " + to_string(id) + ", " + to_string(rating) + ", '" + comment + "')";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Review Submitted Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Submit Review!" << endl;
    }
}

void Worker::signUp()
{
    int id = generateID();
    string name, phone, city, password, category;
    double fees;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Phone: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter City: ";
    cin.ignore();
    getline(cin, city);
    cout << "Enter Password: ";
    cin.ignore();
    getline(cin, password);
    cout << "Enter Work Category: ";
    cin.ignore();
    getline(cin, category);
    cout << "Enter Hire Fees: ";
    cin >> fees;
    string query = "INSERT INTO workers (worker_id, name, phone, city, password, work_category, hire_fees) VALUES (" + to_string(id) + ", '" + name + "', '" + phone + "', '" + city + "', '" + password + "', '" + category + "', " + to_string(fees) + ")";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Sign Up Successful! Your ID is: " << id << endl;
    }
    else
    {
        cout << "Sign Up Failed!" << endl;
    }
}

void Worker::logIn()
{
    int id;
    string password;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;
    string query = "SELECT * FROM workers WHERE worker_id = " + to_string(id) + " AND password = '" + password + "'";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0)
    {
        cout << "Login Successful!" << endl;
        loggedInWorker(id);
    }
    else
    {
        cout << "Login Failed!" << endl;
    }
}

void Worker::loggedInWorker(int id)
{
    int loggedInWorkerChoice;
    do
    {
        cout << "\n========= Logged In Worker Menu =========" << endl;
        cout << "1. View Requests" << endl;
        cout << "2. Respond to Request" << endl;
        cout << "3. Edit Account" << endl;
        cout << "4. Delete Account" << endl;
        cout << "5. View Assigned Tasks" << endl;
        cout << "6. Chat with Customer" << endl;
        cout << "7. View Messages" << endl;
        cout << "0. Log Out" << endl;
        cout << "Enter Your Choice: ";
        cin >> loggedInWorkerChoice;

        if (loggedInWorkerChoice == 1)
        {
            viewRequests(id);
        }
        else if (loggedInWorkerChoice == 2)
        {
            respondToRequest(id);
        }
        else if (loggedInWorkerChoice == 3)
        {
            editAccount(id);
        }
        else if (loggedInWorkerChoice == 4)
        {
            deleteAccount(id);
        }
        else if (loggedInWorkerChoice == 5)
        {
            viewAssignedTasks(id);
        }
        else if (loggedInWorkerChoice == 6)
        {
            chatWithCustomer(id);
        }
        else if (loggedInWorkerChoice == 7)
        {
            viewMessages(id);
        }
    } while (loggedInWorkerChoice != 0);
}

void Worker::viewRequests(int id)
{
    string query = "SELECT * FROM service_requests WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            cout << "Request ID: " << row[0] << ", Customer ID: " << row[1] << ", Description: " << row[3] << ", Fee: " << row[4] << endl;
        }
    }
    else
    {
        cout << "Unable to fetch requests." << endl;
    }
}

void Worker::respondToRequest(int id)
{
    int request_id;
    cout << "Enter Request ID: ";
    cin >> request_id;
    string response;
    cout << "Enter Response: ";
    cin.ignore();
    getline(cin, response);
    string query = "UPDATE service_requests SET response = '" + response + "' WHERE request_id = " + to_string(request_id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Response Sent Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Send Response!" << endl;
    }
}

void Worker::editAccount(int id)
{
    string name, phone, city, password, category;
    double fees;
    cout << "Enter New Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter New Phone: ";
    cin.ignore();
    getline(cin, phone);
    cout << "Enter New City: ";
    cin.ignore();
    getline(cin, city);
    cout << "Enter New Password: ";
    cin.ignore();
    getline(cin, password);
    cout << "Enter New Work Category: ";
    cin.ignore();
    getline(cin, category);
    cout << "Enter New Hire Fees: ";
    cin >> fees;
    string query = "UPDATE workers SET name = '" + name + "', phone = '" + phone + "', city = '" + city + "', password = '" + password + "', work_category = '" + category + "', hire_fees = " + to_string(fees) + " WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout<<"Account Updated Successfully!"<<endl;
    }
    else
    {
        cout<<"Failed to Update Account!"<< endl;
    }
}

void Worker::deleteAccount(int id)
{
    string query = "DELETE FROM workers WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << "Account Deleted Successfully!" << endl;
    }
    else
    {
        cout << "Failed to Delete Account!" << endl;
    }
}

void Worker::viewAssignedTasks(int id)
{
    string query = "SELECT * FROM service_requests WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            cout << "Request ID: " << row[0] << ", Customer ID: " << row[1] << ", Description: " << row[3] << ", Fee: " << row[4] << endl;
        }
    }
    else
    {
        cout << "Unable to fetch assigned tasks." << endl;
    }
}

void Worker::chatWithCustomer(int id)
{
    string query = "SELECT request_id, customer_id FROM service_requests WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if (mysql_num_rows(res) > 0)
        {
            cout << "Select a request to chat with the customer:" << endl;
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Request ID: " << row[0] << ", Customer ID: " << row[1] << endl;
            }
            int request_id;
            cout << "Enter Request ID: ";
            cin >> request_id;
            string message;
            cout << "Enter Message: ";
            cin.ignore();
            getline(cin, message);
            query = "INSERT INTO chats (request_id, sender_role, message) VALUES (" + to_string(request_id) + ", 'worker', '" + message + "')";
            q = query.c_str();
            qstate = mysql_query(conn, q);
            if (!qstate)
            {
                cout << "Message Sent to Customer!" << endl;
            }
            else
            {
                cout << "Failed to Send Message!" << endl;
                cout << "MySQL Error: " << mysql_error(conn) << endl;
            }
        }
        else
        {
            cout << "No requests found." << endl;
        }
    }
    else
    {
        cout << "Failed to fetch requests." << endl;
    }
}

void Worker::viewMessages(int id)
{
    string query = "SELECT c.request_id, c.message, c.timestamp, sr.customer_id FROM chats c JOIN service_requests sr ON c.request_id = sr.request_id WHERE sr.worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if (mysql_num_rows(res) > 0)
        {
            cout << "Messages:" << endl;
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Request ID: " << row[0] << ", Timestamp: " << row[2] << ", Customer ID: " << row[3] << endl;
                cout << "\tMessage: " << row[1] << endl;
                ;
            }
        }
        else
        {
            cout << "No messages found." << endl;
        }
    }
    else
    {
        cout << "Failed to Fetch Messages!" << endl;
        cout << "MySQL Error: " << mysql_error(conn) << endl;
    }
}

int main()
{
    connectDatabase();

    int choice;
    do
    {
        cout << "\n========= Service Management System =========" << endl;
        cout << "1. Customer" << endl;
        cout << "2. Worker" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            Customer c;
            int customerChoice;
            do
            {
                cout << "\n========= Customer Menu =========" << endl;
                cout << "1. Sign Up" << endl;
                cout << "2. Log In" << endl;
                cout << "0. Back" << endl;
                cout << "Enter Your Choice: ";
                cin >> customerChoice;

                if (customerChoice == 1)
                {
                    c.signUp();
                }
                else if (customerChoice == 2)
                {
                    c.logIn();
                }
            } while (customerChoice != 0);
        }
        else if (choice == 2)
        {
            Worker w;
            int workerChoice;
            do
            {
                cout << "\n========= Worker Menu =========" << endl;
                cout << "1. Sign Up" << endl;
                cout << "2. Log In" << endl;
                cout << "0. Back" << endl;
                cout << "Enter Your Choice: ";
                cin >> workerChoice;

                if (workerChoice == 1)
                {
                    w.signUp();
                }
                else if (workerChoice == 2)
                {
                    w.logIn();
                }
            } while (workerChoice != 0);
        }
    } while (choice != 0);

    mysql_close(conn);
    return 0;
}
