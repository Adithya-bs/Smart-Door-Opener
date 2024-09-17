# Smart Door Opener 
## Description

The Smart Door Opener project is designed to automate door operation using an ESP32 microcontroller combined with a distance sensor. This system provides remote access control functionality, allowing users to open or close their door from anywhere through a web interface.

## Technologies Used

### Hardware

- **ESP32 Microcontroller**: Acts as the central controller for the Smart Door Opener system, handling the communication between the distance sensor and the web application.

### Software

- **C**: Programming language used for developing the firmware for the ESP32 microcontroller.
- **Node.js**: JavaScript runtime used to create the backend server that interfaces with the ESP32.
- **Express.js**: Web framework for Node.js, used to build RESTful APIs for communication between the backend server and the web interface.
- **WebSocket**: Protocol used to enable real-time, two-way communication between the server and the web client.
- **HTML**: Markup language for structuring the web interface.
- **CSS**: Stylesheet language used to style the web interface.
- **JavaScript**: Programming language used for client-side scripting and enhancing the interactive aspects of the web interface.

## Installation

1. **Clone the repository**:

    ```bash
    git clone https://github.com/your-username/smart-door-opener.git
    ```

2. **Set up the hardware**:

    - Connect the ESP32 microcontroller to your computer.
    - Attach the distance sensor to the ESP32 as per the hardware schematic provided in the repository.

3. **Upload Firmware**:

    - Use the Arduino IDE or PlatformIO to upload the firmware to the ESP32. Ensure you have the necessary libraries and drivers installed.

4. **Install Backend Dependencies**:

    - Navigate to the backend directory of the project and run:

    ```bash
    npm install
    ```

5. **Start the Backend Server**:

    - Run the backend server using:

    ```bash
    npm start
    ```

6. **Set up the Web Interface**:

    - Navigate to the web directory and open `index.html` in a web browser, or use a local server to serve the HTML files.

7. **Configure Environment Variables**:

    - Ensure that any necessary environment variables are set up according to the `.env` file in the backend directory.

8. **Access the Application**:

    - Open the web interface in your browser. The application should connect to the ESP32 and allow you to control the door remotely.

## Future Enhancements

- Implement user authentication for secure access control.
- Integrate additional sensors for enhanced security features.
- Add voice control capabilities through integration with voice assistants.
- Enhance the web interface with more advanced controls and monitoring options.
