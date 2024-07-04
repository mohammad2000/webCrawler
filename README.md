# Web Crawler with Qt

This repository contains a simple web crawler built using the Qt framework. The crawler is designed to navigate through web pages, extract data, and store it for further processing. The project uses MongoDB for data storage and RabbitMQ for message queuing, and it is designed to follow a microservices architecture.

## Features

- **Multithreading:** Efficiently crawls multiple web pages simultaneously.
- **HTML Parsing:** Extracts data from HTML documents using Qt's powerful parsing capabilities.
- **MongoDB Integration:** Stores extracted data in a MongoDB database.
- **RabbitMQ Integration:** Uses RabbitMQ for message queuing to manage crawling tasks.
- **Microservices Architecture:** Modular design for scalability and maintainability.

## Prerequisites

- Qt 5.15 or later
- C++ Compiler (e.g., GCC, Clang, MSVC)
- CMake (optional, but recommended for building the project)
- MongoDB
- RabbitMQ
- Docker (optional, for running services in containers)

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/mohammad2000/web-crawler-qt.git
   cd web-crawler-qt
Open the project in Qt Creator:

Launch Qt Creator.
Open the project by selecting File -> Open File or Project and navigating to the cloned directory.
Select the CMakeLists.txt or web-crawler-qt.pro file.
Build the project:

Configure the project with your preferred kit.
Build the project by clicking the build button or selecting Build -> Build Project.
Set up MongoDB and RabbitMQ:

Install MongoDB and RabbitMQ on your local machine or use Docker to run them in containers.
bash
Copy code
docker run -d -p 27017:27017 --name mongodb mongo
docker run -d -p 5672:5672 -p 15672:15672 --name rabbitmq rabbitmq:3-management
Usage

Run the application:

Execute the built binary from Qt Creator or from the command line.
bash
Copy code
./web-crawler-qt
Configure the crawler:

Edit the config.json file to specify the starting URL, crawl depth, MongoDB connection details, and RabbitMQ connection details.
json
Copy code
{
  "start_url": "https://example.com",
  "max_depth": 3,
  "mongodb_uri": "mongodb://localhost:27017",
  "rabbitmq_uri": "amqp://localhost"
}
Start crawling:

The application will start crawling the specified website, queue tasks using RabbitMQ, and store the extracted data in MongoDB.
Configuration

The web crawler can be configured using a config.json file. Below are the configurable parameters:

start_url: The URL from which the crawler will start.
max_depth: The maximum depth the crawler will navigate to from the starting URL.
mongodb_uri: The MongoDB connection URI.
rabbitmq_uri: The RabbitMQ connection URI.
Microservices

This project follows a microservices architecture:

Crawler Service: Handles the crawling of web pages.
Queue Service: Manages the message queue using RabbitMQ.
Storage Service: Stores the extracted data in MongoDB.
Contributing

Contributions are welcome! Please fork the repository and create a pull request with your changes. Ensure your code adheres to the existing coding standards and includes appropriate tests.

License

This project is licensed under the MIT License. See the LICENSE file for details.

Acknowledgements

This project was inspired by the need for efficient web data extraction and leverages the powerful features of the Qt framework for multithreading and HTML parsing.

For more information, feel free to contact Mohammad Salamati.
m.salamati2000@gmail.com
(514) 619-4662

Happy Crawling!

