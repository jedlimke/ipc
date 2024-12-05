# IPC Project

This project demonstrates inter-process communication (IPC) in C++ using POSIX message queues. The setup includes a Dockerized Ubuntu 22.04 environment as requested. The development environment goes hand-in-hand with the container, allowing direct development within Ubuntu from any external HOST platform.

---

## **Getting Started**

### **Why Docker Compose?**

Although this project only uses a single container, Docker Compose simplifies the workflow by:
- Automating container creation and configuration.
- Mapping the project directory into the container for seamless code editing.
- Providing a single command to build and run the environment.


### **Setup Instructions**

1. **Build and Start the Docker Container**:
   Stay attached:
   ```bash
   docker-compose up --build
   ```
   OR do NOT stay attached:
   ```bash
   docker-compose up -d --build
   ```
   This will:
   - Build an Ubuntu 22.04-based development container.
   - Map the project directory into the container under `/app`.

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Stop the Container**:
   ```bash
   docker-compose down
   ```
