#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <mysql.h>
#include <conio.h>
#include <ctime>
#include <sstream>
void cls()
{
    system("cls");
}

void cls()
{
    system("clear");
}


using namespace std;

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"

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
        cout << GREEN << ITALIC << "Database Connected Successfully!" << RESET << endl;
    }
    else
    {
        cout << RED << ITALIC << "Failed to Connect!" << RESET << endl;
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
    void updateTaskStatus(int id);
};

void Customer::signUp()
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "             Sign Up              " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    int id = generateID();
    string name, phone, city, password;
    cout << ITALIC << "Enter Name: " << RESET;
    cin.ignore();
    getline(cin, name);
    cout << ITALIC << "Enter Phone: " << RESET;
    cin >> phone;
    cin.ignore();
    cout << ITALIC << "Enter City: " << RESET;
    getline(cin, city);
    cout << ITALIC << "Enter Password: " << RESET;
	password = "";
	char ch;
	while (true) 
	{
	    ch = getch();
	    if (ch == 13) break;
	    else if (ch == 8) 
		{ 
	        if (!password.empty()) 
			{
			    password.erase(password.size() - 1);
			    cout << "\b \b";
			}
	    } 
		else 
		{
	        password += ch;
	        cout << '*';
	    }
	}
	cout<<endl;
    string query = "INSERT INTO customers (customer_id, name, phone, city, password) VALUES (" + to_string(id) + ", '" + name + "', '" + phone + "', '" + city + "', '" + password + "')";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << GREEN << ITALIC << "Sign Up Successful! Your ID is: " << id << RESET << endl;
    }
    else
    {
        cout << RED << ITALIC << "Sign Up Failed!" << RESET << endl;
    }
    cin.ignore();
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::logIn()
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "             Log In               " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    int id;
    string password;
    cout << ITALIC << "Enter ID: " << RESET;
    cin >> id;
    cout << ITALIC << "Enter Password: " << RESET;
	password = "";
	char ch;
	while (true) 
	{
	    ch = getch();
	    if (ch == 13) break;
	    else if (ch == 8) 
		{ 
	        if (!password.empty()) 
			{
			    password.erase(password.size() - 1);
			    cout << "\b \b";
			}
	    } 
		else 
		{
	        password += ch;
	        cout << '*';
	    }
	}
	cout<<endl;
	
    string query = "SELECT * FROM customers WHERE customer_id = " + to_string(id) + " AND password = '" + password + "'";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0)
    {
        cout << GREEN << ITALIC << "Login Successful!" << RESET << endl;
        loggedInCustomer(id);
    }
    else
    {
        cout << RED << ITALIC << "Login Failed!" << RESET << endl;
    }
    cin.ignore();
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::loggedInCustomer(int id)
{
    int loggedInCustomerChoice;
    do
    {
        cout << BOLD << CYAN << "\n\n======================================" << endl;
        cout << BOLD << CYAN << "      Logged In Customer Menu      " << endl;
        cout << BOLD << CYAN << "======================================" << endl;
        cout << ITALIC << "1. Request Service" << endl;
        cout << "2. Edit Account" << endl;
        cout << "3. Delete Account" << endl;
        cout << "4. View Assigned Tasks" << endl;
        cout << "5. Chat with Worker" << endl;
        cout << "6. Give Review" << endl;
        cout << "7. View Messages" << endl;
        cout << "8. Search Workers" << endl;
        cout << "0. Log Out" << RESET << endl;
        cout << ITALIC << "Enter Your Choice: " << RESET;
        cin >> loggedInCustomerChoice;

        switch (loggedInCustomerChoice)
        {
        case 1:
            requestService(id);
            break;
        case 2:
            editAccount(id);
            break;
        case 3:
            deleteAccount(id);
            break;
        case 4:
            viewAssignedTasks(id);
            break;
        case 5:
            chatWithWorker(id);
            break;
        case 6:
            giveReview(id);
            break;
        case 7:
            viewMessages(id);
            break;
        case 8:
            searchWorkers();
            break;
        case 0:
            break;
        default:
            cout << RED << "Invalid choice!" << RESET << endl;
            cin.ignore();
            cout << ITALIC << "Press Enter to continue..." << RESET;
            cin.get();
        }
    } while (loggedInCustomerChoice != 0);
}

void Customer::requestService(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Request Service          " << endl;
    cout << BOLD << CYAN << "======================================" << endl;

    string city, category, description;
    double proposed_fee;

    cout << ITALIC << "Enter City: " << RESET;
    cin.ignore();
    getline(cin, city);

    cout << ITALIC << "Enter Service Category: " << RESET;
    getline(cin, category);

    cout << ITALIC << "Enter Description: " << RESET;
    getline(cin, description);

    cout << ITALIC << "Enter Proposed Fee: " << RESET;
    cin >> proposed_fee;
    cin.ignore();

    // Find available workers
    string workerQuery = "SELECT worker_id, name, hire_fees, reviews FROM workers "
                        "WHERE city = '" + city + "' AND work_category = '" + category + "'";
    
    const char* q = workerQuery.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES* res = mysql_store_result(conn);
        int numWorkers = mysql_num_rows(res);
        
        if (numWorkers == 0)
        {
            cout << RED << "\nNo available workers for this service in " << city << RESET << endl;
            cout << ITALIC << "Press Enter to continue..." << RESET;
            cin.get();
            return;
        }
        
        cout << "\n" << GREEN << "Available Workers:" << RESET << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Fees" << setw(10) << "Rating" << endl;
        cout << string(50, '-') << endl;
        
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(10) << row[0] << setw(20) << row[1] << setw(10) << row[2] << setw(10) << row[3] << endl;
        }
        mysql_free_result(res);
        
        // Select a worker
        int worker_id;
        cout << "\n" << ITALIC << "Enter Worker ID to assign (0 to cancel): " << RESET;
        cin >> worker_id;
        cin.ignore();
        
        if (worker_id == 0)
        {
            cout << YELLOW << "Request cancelled." << RESET << endl;
            cout << ITALIC << "Press Enter to continue..." << RESET;
            cin.get();
            return;
        }
        
        // Create service request
        string insertQuery = "INSERT INTO service_requests "
                           "(customer_id, city, category, worker_id, description, proposed_fee, status) "
                           "VALUES (" + to_string(id) + ", '" + city + "', '" + category + "', " + 
                           to_string(worker_id) + ", '" + description + "', " + 
                           to_string(proposed_fee) + ", 'pending')";
        
        qstate = mysql_query(conn, insertQuery.c_str());
        
        if (!qstate)
        {
            cout << GREEN << "Service request created successfully!" << RESET << endl;
        }
        else
        {
            cout << RED << "Failed to create service request: " << mysql_error(conn) << RESET << endl;
        }
    }
    else
    {
        cout << RED << "Error finding workers: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::viewAssignedTasks(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "       View Assigned Tasks         " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    string query = "SELECT sr.request_id, w.name, sr.description, sr.proposed_fee, sr.status "
                  "FROM service_requests sr "
                  "JOIN workers w ON sr.worker_id = w.worker_id "
                  "WHERE sr.customer_id = " + to_string(id);
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << left << setw(10) << "Req ID" << setw(20) << "Worker" 
                 << setw(30) << "Description" << setw(10) << "Fee" << setw(15) << "Status" << endl;
            cout << string(85, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(20) << row[1] 
                     << setw(30) << (string(row[2]).substr(0, 27) + (strlen(row[2]) > 27 ? "..." : ""))
                     << setw(10) << row[3] << setw(15) << row[4] << endl;
            }
        }
        else
        {
            cout << YELLOW << "No assigned tasks found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching tasks: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::chatWithWorker(int id)
{
    cout << BOLD << CYAN << "\n======================================" << endl;
    cout << BOLD << CYAN << "          Chat with Worker         " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    // Show active requests
    string query = "SELECT sr.request_id, w.name, sr.status "
                  "FROM service_requests sr "
                  "JOIN workers w ON sr.worker_id = w.worker_id "
                  "WHERE sr.customer_id = " + to_string(id);
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << ITALIC << "Your active requests:" << RESET << endl;
            cout << left << setw(10) << "Req ID" << setw(20) << "Worker" << setw(15) << "Status" << endl;
            cout << string(45, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(20) << row[1] << setw(15) << row[2] << endl;
            }
            
            int request_id;
            cout << "\n" << ITALIC << "Enter Request ID to chat (0 to cancel): " << RESET;
            cin >> request_id;
            cin.ignore();
            
            if (request_id != 0)
            {
                string message;
                cout << ITALIC << "Enter your message: " << RESET;
                getline(cin, message);
                
                string insertQuery = "INSERT INTO chats (request_id, sender_role, message) "
                                   "VALUES (" + to_string(request_id) + ", 'customer', '" + message + "')";
                
                qstate = mysql_query(conn, insertQuery.c_str());
                
                if (!qstate)
                {
                    cout << GREEN << "Message sent successfully!" << RESET << endl;
                }
                else
                {
                    cout << RED << "Failed to send message: " << mysql_error(conn) << RESET << endl;
                }
            }
        }
        else
        {
            cout << YELLOW << "No active requests found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching requests: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::viewMessages(int id)
{

    cout << BOLD << CYAN << "\n======================================" << endl;
    cout << BOLD << CYAN << "           View Messages           " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    // Get all requests for this customer
    string query = "SELECT c.chat_id, c.request_id, c.sender_role, c.message, c.timestamp "
                  "FROM chats c "
                  "JOIN service_requests sr ON c.request_id = sr.request_id "
                  "WHERE sr.customer_id = " + to_string(id) + " "
                  "ORDER BY c.timestamp DESC";
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << left << setw(10) << "Req ID" << setw(10) << "Sender" 
                 << setw(20) << "Time" << setw(40) << "Message" << endl;
            cout << string(80, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                string timestamp = row[4];
                timestamp = timestamp.substr(0, timestamp.find(".")); // Remove milliseconds
                
                cout << setw(10) << row[1] << setw(10) << row[2] 
                     << setw(20) << timestamp << setw(40) 
                     << (string(row[3]).substr(0, 37) + (strlen(row[3]) > 37 ? "..." : "")) << endl;
            }
        }
        else
        {
            cout << YELLOW << "No messages found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching messages: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::giveReview(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "            Give Review            " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    // Get completed services
    string query = "SELECT sr.request_id, w.worker_id, w.name "
                  "FROM service_requests sr "
                  "JOIN workers w ON sr.worker_id = w.worker_id "
                  "WHERE sr.customer_id = " + to_string(id) + " "
                  "AND sr.status = 'completed'";
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << ITALIC << "Completed services:" << RESET << endl;
            cout << left << setw(10) << "Req ID" << setw(10) << "Worker ID" << setw(20) << "Worker Name" << endl;
            cout << string(40, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(10) << row[1] << setw(20) << row[2] << endl;
            }
            
            int worker_id;
            cout << "\n" << ITALIC << "Enter Worker ID to review: " << RESET;
                        cin >> worker_id;
            cin.ignore();
            
            double rating;
            string comment;
            cout << ITALIC << "Enter rating (1-5): " << RESET;
            cin >> rating;
            cin.ignore();
            cout << ITALIC << "Enter your comment: " << RESET;
            getline(cin, comment);
            
            // Insert review
            string insertQuery = "INSERT INTO reviews (worker_id, customer_id, rating, comment) "
                               "VALUES (" + to_string(worker_id) + ", " + to_string(id) + ", " 
                               + to_string(rating) + ", '" + comment + "')";
            
            qstate = mysql_query(conn, insertQuery.c_str());
            
            if (!qstate)
            {
                // Update worker's average rating
                string updateQuery = "UPDATE workers SET reviews = ("
                                   "SELECT AVG(rating) FROM reviews WHERE worker_id = " + to_string(worker_id) + ") "
                                   "WHERE worker_id = " + to_string(worker_id);
                mysql_query(conn, updateQuery.c_str());
                
                cout << GREEN << "Review submitted successfully!" << RESET << endl;
            }
            else
            {
                cout << RED << "Failed to submit review: " << mysql_error(conn) << RESET << endl;
            }
        }
        else
        {
            cout << YELLOW << "No completed services found to review." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching services: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::signUp()
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "             Sign Up              " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    int id = generateID();
    string name, phone, city, password, work_category;
    double hire_fees;
    cout << ITALIC << "Enter Name: " << RESET;
    cin.ignore();
    getline(cin, name);
    cout << ITALIC << "Enter Phone: " << RESET;
    getline(cin, phone);
    cout << ITALIC << "Enter City: " << RESET;
    getline(cin, city);
    cout << ITALIC << "Enter Password: " << RESET;
	password = "";
	char ch;
	while (true) 
	{
	    ch = getch();
	    if (ch == 13) break;
	    else if (ch == 8) 
		{ 
	        if (!password.empty()) 
			{
			    password.erase(password.size() - 1);
			    cout << "\b \b";
			}
	    } 
		else 
		{
	        password += ch;
	        cout << '*';
	    }
	}
    cout<<endl;
    cout << ITALIC << "Enter Work Category: " << RESET;
    getline(cin, work_category);
    cout << ITALIC << "Enter Hire Fees: " << RESET;
    cin >> hire_fees;
    string query = "INSERT INTO workers (worker_id, name, phone, city, password, work_category, hire_fees) VALUES (" + to_string(id) + ", '" + name + "', '" + phone + "', '" + city + "', '" + password + "', '" + work_category + "', " + to_string(hire_fees) + ")";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate)
    {
        cout << GREEN << ITALIC << "Sign Up Successful! Your ID is: " << id << RESET << endl;
    }
    else
    {
        cout << RED << ITALIC << "Sign Up Failed!" << RESET << endl;
    }
    cin.ignore();
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::logIn()
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "             Log In               " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    int id;
    string password;
    cout << ITALIC << "Enter ID: " << RESET;
    cin >> id;
    cout << ITALIC << "Enter Password: " << RESET;
	password = "";
	char ch;
	while (true) 
	{
	    ch = getch();
	    if (ch == 13) break;
	    else if (ch == 8) 
		{ 
	        if (!password.empty()) 
			{
			    password.erase(password.size() - 1);
			    cout << "\b \b";
			}
	    } 
		else 
		{
	        password += ch;
	        cout << '*';
	    }
	}
	cout << endl;

    string query = "SELECT * FROM workers WHERE worker_id = " + to_string(id) + " AND password = '" + password + "'";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0)
    {
        cout << GREEN << ITALIC << "Login Successful!" << RESET << endl;
        loggedInWorker(id);
    }
    else
    {
        cout << RED << ITALIC << "Login Failed!" << RESET << endl;
    }
    cin.ignore();
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::deleteAccount(int id) 
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Delete Account           " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    string query = "DELETE FROM customers WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate) 
	{
        cout << GREEN << ITALIC << "Account Deleted Successfully!" << RESET << endl;
    } 
	else 
	{
        cout << RED << ITALIC << "Failed to Delete Account!" << RESET << endl;
    }
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::searchWorkers() 
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Search Workers           " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    string city, category;
    cout << ITALIC << "Enter city: " << RESET;
    cin.ignore();
    getline(cin, city);
    cout << ITALIC << "Enter category: " << RESET;
    getline(cin, category);
    string query = "SELECT * FROM workers WHERE city = '" + city + "' AND work_category = '" + category + "'";
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate) {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        if (numRows > 0) 
		{
            cout << GREEN << ITALIC << "Workers found:" << RESET << endl;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) 
			{
                cout << "Worker ID: " << row[0] << endl;
                cout << "Name: " << row[1] << endl;
                cout << "Phone: " << row[2] << endl;
                cout << "City: " << row[3] << endl;
                cout << "Work Category: " << row[5] << endl;
                cout << "Hire Fees: " << row[6] << endl;
                cout << endl;
            }
        } else {
            cout << YELLOW << "No workers found." << RESET << endl;
        }
        mysql_free_result(res);
    } else {
        cout << RED << "Error searching workers: " << mysql_error(conn) << RESET << endl;
    }
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::editAccount(int id) 
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "           Edit Account            " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    string name, phone, city, password, work_category;
    double hire_fees;
    cout << ITALIC << "Enter new name: " << RESET;
    cin.ignore();
    getline(cin, name);
    cout << ITALIC << "Enter new phone: " << RESET;
    getline(cin, phone);
    cout << ITALIC << "Enter new city: " << RESET;
    getline(cin, city);
    cout << ITALIC << "Enter new password: " << RESET;
    cin >> password;
    cin.ignore();
    cout << ITALIC << "Enter new work category: " << RESET;
    getline(cin, work_category);
    cout << ITALIC << "Enter new hire fees: " << RESET;
    cin >> hire_fees;
    string query = "UPDATE workers SET name = '" + name + "', phone = '" + phone + "', city = '" + city + "', password = '" + password + "', work_category = '" + work_category + "', hire_fees = " + to_string(hire_fees) + " WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate) 
	{
        cout << GREEN << ITALIC << "Account updated successfully!" << RESET << endl;
    } 
	else 
	{
        cout << RED << ITALIC << "Failed to update account!" << RESET << endl;
    }
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::deleteAccount(int id) 
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Delete Account           " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    string query = "DELETE FROM workers WHERE worker_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate) 
	{
        cout << GREEN << ITALIC << "Account deleted successfully!" << RESET << endl;
    } 
	else 
	{
        cout << RED << ITALIC << "Failed to delete account!" << RESET << endl;
    }
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Customer::editAccount(int id) 
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "           Edit Account            " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    string name, phone, city, password;
    cout << ITALIC << "Enter new name: " << RESET;
    cin.ignore();
    getline(cin, name);
    cout << ITALIC << "Enter new phone: " << RESET;
    getline(cin, phone);
    cout << ITALIC << "Enter new city: " << RESET;
    getline(cin, city);
    cout << ITALIC << "Enter new password: " << RESET;
    getline(cin, password);
    string query = "UPDATE customers SET name = '" + name + "', phone = '" + phone + "', city = '" + city + "', password = '" + password + "' WHERE customer_id = " + to_string(id);
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    if (!qstate) {
        cout << GREEN << ITALIC << "Account updated successfully!" << RESET << endl;
    } else {
        cout << RED << ITALIC << "Failed to update account!" << RESET << endl;
    }
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}
void Worker::loggedInWorker(int id)
{
    int loggedInWorkerChoice;
    do
    {

        cout << BOLD << CYAN << "\n\n======================================" << endl;
        cout << BOLD << CYAN << "      Logged In Worker Menu        " << endl;
        cout << BOLD << CYAN << "======================================" << endl;
        cout << ITALIC << "1. View Requests" << endl;
        cout << "2. Respond to Request" << endl;
        cout << "3. Edit Account" << endl;
        cout << "4. Delete Account" << endl;
        cout << "5. View Assigned Tasks" << endl;
        cout << "6. Chat with Customer" << endl;
        cout << "7. View Messages" << endl;
        cout << "8. Update Task Status" << endl;
        cout << "0. Log Out" << RESET << endl;
        cout << ITALIC << "Enter Your Choice: " << RESET;
        cin >> loggedInWorkerChoice;

        switch (loggedInWorkerChoice)
        {
        case 1:
            viewRequests(id);
            break;
        case 2:
            respondToRequest(id);
            break;
        case 3:
            editAccount(id);
            break;
        case 4:
            deleteAccount(id);
            break;
        case 5:
            viewAssignedTasks(id);
            break;
        case 6:
            chatWithCustomer(id);
            break;
        case 7:
            viewMessages(id);
            break;
        case 8:
            updateTaskStatus(id);
            break;
        case 0:
            break;
        default:
            cout << RED << "Invalid choice!" << RESET << endl;
            cin.ignore();
            cout << ITALIC << "Press Enter to continue..." << RESET;
            cin.get();
        }
    } while (loggedInWorkerChoice != 0);
}

void Worker::viewRequests(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Available Requests        " << endl;
    cout << BOLD << CYAN << "======================================" << endl;

    string query = "SELECT sr.request_id, c.name, sr.description, sr.proposed_fee "
                  "FROM service_requests sr "
                  "JOIN customers c ON sr.customer_id = c.customer_id "
                  "WHERE sr.worker_id = " + to_string(id) + " "
                  "AND sr.status = 'pending'";
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << ITALIC << "Pending requests assigned to you:" << RESET << endl;
            cout << left << setw(10) << "Req ID" << setw(20) << "Customer" 
                 << setw(30) << "Description" << setw(10) << "Fee" << endl;
            cout << string(70, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(20) << row[1] 
                     << setw(30) << (string(row[2]).substr(0, 27) + (strlen(row[2]) > 27 ? "..." : ""))
                     << setw(10) << row[3] << endl;
            }
        }
        else
        {
            cout << YELLOW << "No pending requests found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching requests: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::respondToRequest(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "        Respond to Request         " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    int request_id;
    cout << ITALIC << "Enter Request ID to respond: " << RESET;
    cin >> request_id;
    cin.ignore();
    
    string response;
    cout << ITALIC << "Enter your response: " << RESET;
    getline(cin, response);
    
    string query = "INSERT INTO chats (request_id, sender_role, message) "
                  "VALUES (" + to_string(request_id) + ", 'worker', '" + response + "')";
    
    int qstate = mysql_query(conn, query.c_str());
    
    if (!qstate)
    {
        cout << GREEN << "Response sent successfully!" << RESET << endl;
    }
    else
    {
        cout << RED << "Failed to send response: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::viewAssignedTasks(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "       View Assigned Tasks         " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    string query = "SELECT sr.request_id, c.name, sr.description, sr.proposed_fee, sr.status "
                  "FROM service_requests sr "
                  "JOIN customers c ON sr.customer_id = c.customer_id "
                  "WHERE sr.worker_id = " + to_string(id);
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << left << setw(10) << "Req ID" << setw(20) << "Customer" 
                 << setw(30) << "Description" << setw(10) << "Fee" << setw(15) << "Status" << endl;
            cout << string(85, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(20) << row[1] 
                     << setw(30) << (string(row[2]).substr(0, 27) + (strlen(row[2]) > 27 ? "..." : ""))
                     << setw(10) << row[3] << setw(15) << row[4] << endl;
            }
        }
        else
        {
            cout << YELLOW << "No assigned tasks found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching tasks: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::chatWithCustomer(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "          Chat with Customer       " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    // Show active requests
    string query = "SELECT sr.request_id, c.name, sr.status "
                  "FROM service_requests sr "
                  "JOIN customers c ON sr.customer_id = c.customer_id "
                  "WHERE sr.worker_id = " + to_string(id);
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << ITALIC << "Your active requests:" << RESET << endl;
            cout << left << setw(10) << "Req ID" << setw(20) << "Customer" << setw(15) << "Status" << endl;
            cout << string(45, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(10) << row[0] << setw(20) << row[1] << setw(15) << row[2] << endl;
            }
            
            int request_id;
            cout << "\n" << ITALIC << "Enter Request ID to chat (0 to cancel): " << RESET;
            cin >> request_id;
            cin.ignore();
            
            if (request_id != 0)
            {
                string message;
                cout << ITALIC << "Enter your message: " << RESET;
                getline(cin, message);
                
                string insertQuery = "INSERT INTO chats (request_id, sender_role, message) "
                                   "VALUES (" + to_string(request_id) + ", 'worker', '" + message + "')";
                
                qstate = mysql_query(conn, insertQuery.c_str());
                
                if (!qstate)
                {
                    cout << GREEN << "Message sent successfully!" << RESET << endl;
                }
                else
                {
                    cout << RED << "Failed to send message: " << mysql_error(conn) << RESET << endl;
                }
            }
        }
        else
        {
            cout << YELLOW << "No active requests found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching requests: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::viewMessages(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "           View Messages           " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    string query = "SELECT c.chat_id, c.request_id, c.sender_role, c.message, c.timestamp "
                  "FROM chats c "
                  "JOIN service_requests sr ON c.request_id = sr.request_id "
                  "WHERE sr.worker_id = " + to_string(id) + " "
                  "ORDER BY c.timestamp DESC";
    
    const char *q = query.c_str();
    int qstate = mysql_query(conn, q);
    
    if (!qstate)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int numRows = mysql_num_rows(res);
        
        if (numRows > 0)
        {
            cout << left << setw(10) << "Req ID" << setw(10) << "Sender" 
                 << setw(20) << "Time" << setw(40) << "Message" << endl;
            cout << string(80, '-') << endl;
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                string timestamp = row[4];
                timestamp = timestamp.substr(0, timestamp.find(".")); 
                
                cout << setw(10) << row[1] << setw(10) << row[2] 
                     << setw(20) << timestamp << setw(40) 
                     << (string(row[3]).substr(0, 37) + (strlen(row[3]) > 37 ? "..." : "")) << endl;
            }
        }
        else
        {
            cout << YELLOW << "No messages found." << RESET << endl;
        }
        mysql_free_result(res);
    }
    else
    {
        cout << RED << "Error fetching messages: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
}

void Worker::updateTaskStatus(int id)
{
    cls();
    cout << BOLD << CYAN << "======================================" << endl;
    cout << BOLD << CYAN << "        Update Task Status         " << endl;
    cout << BOLD << CYAN << "======================================" << endl;
    
    int request_id;
    cout << ITALIC << "Enter Request ID to update status: " << RESET;
    cin >> request_id;
    cin.ignore();
    
    string status;
    cout << ITALIC << "Enter new status (pending/accepted/denied/completed): " << RESET;
    getline(cin, status);
    
    string query = "UPDATE service_requests SET status = '" + status + "' "
                  "WHERE request_id = " + to_string(request_id);
    
    int qstate = mysql_query(conn, query.c_str());
    
    if (!qstate)
    {
        cout << GREEN << "Task status updated successfully!" << RESET << endl;
    }
    else
    {
        cout << RED << "Failed to update task status: " << mysql_error(conn) << RESET << endl;
    }
    
    cout << ITALIC << "Press Enter to continue..." << RESET;
    cin.get();
} 

int main()
{
    connectDatabase();
    
    int choice;
    do
    {
        cls();
        cout << YELLOW
         << "        __        __  _____   _        ____    ___    __  __   _____ \n"
            "        \\ \\      / / | ____| | |      / ___|  / _ \\  |  \\/  | | ____|\n"
            "         \\ \\ /\\ / /  |  _|   | |     | |     | | | | | |\\/| | |  _|  \n"
            "          \\ V  V /   | |___  | |___  | |___  | |_| | | |  | | | |___ \n"
            "           \\_/\\_/    |_____| |_____|  \\____|  \\___/  |_|  |_| |_____|\n"
        << RESET;
	    cout << YELLOW << "\n\n=================================================================\n\n"
	         << RESET;
        cout << BOLD << CYAN << "\n\n======================================" << endl;
        cout << BOLD << CYAN << "          Service App Menu          " << endl;
        cout << BOLD << CYAN << "======================================" << endl;
        cout << ITALIC << "1. Customer Sign Up" << endl;
        cout << "2. Customer Log In" << endl;
        cout << "3. Worker Sign Up" << endl;
        cout << "4. Worker Log In" << endl;
        cout << "0. Exit" << RESET << endl;
        cout << ITALIC << "Enter Your Choice: " << RESET;
        cin >> choice;
        
        switch (choice)
        {
        case 1:
        {
            Customer customer;
            customer.signUp();
            break;
        }
        case 2:
        {
            Customer customer;
            customer.logIn();
            break;
        }
        case 3:
        {
            Worker worker;
            worker.signUp();
            break;
        }
        case 4:
        {
            Worker worker;
            worker.logIn();
            break;
        }
        case 0:
            break;
        default:
            cout << RED << "Invalid choice!" << RESET << endl;
            cin.ignore();
            cout << ITALIC << "Press Enter to continue..." << RESET;
            cin.get();
        }
    } while (choice != 0);
    
    mysql_close(conn);
    return 0;
}
