CREATE DATABASE IF NOT EXISTS service_app;
USE service_app;

CREATE TABLE IF NOT EXISTS customers (
    customer_id INT PRIMARY KEY,
    name VARCHAR(100),
    phone VARCHAR(15),
    city VARCHAR(50),
    password VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS workers (
    worker_id INT PRIMARY KEY,
    name VARCHAR(100),
    phone VARCHAR(15),
    city VARCHAR(50),
    password VARCHAR(100),
    work_category VARCHAR(50),
    hire_fees DECIMAL(10,2),
    reviews DECIMAL(3,2) DEFAULT 0.00
);

CREATE TABLE IF NOT EXISTS service_requests (
    request_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT,
    worker_id INT,
    description TEXT,
    proposed_fee DECIMAL(10,2),
    status ENUM('pending', 'accepted', 'denied', 'completed') DEFAULT 'pending',
    FOREIGN KEY (customer_id) REFERENCES customers(customer_id) ON DELETE CASCADE,
    FOREIGN KEY (worker_id) REFERENCES workers(worker_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS chats (
    chat_id INT AUTO_INCREMENT PRIMARY KEY,
    request_id INT,
    sender_role ENUM('customer', 'worker'),
    message TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (request_id) REFERENCES service_requests(request_id)
);

CREATE TABLE IF NOT EXISTS reviews (
    review_id INT AUTO_INCREMENT PRIMARY KEY,
    worker_id INT,
    customer_id INT,
    rating DECIMAL(3,2),
    comment TEXT,
    FOREIGN KEY (worker_id) REFERENCES workers(worker_id),
    FOREIGN KEY (customer_id) REFERENCES customers(customer_id)
);

