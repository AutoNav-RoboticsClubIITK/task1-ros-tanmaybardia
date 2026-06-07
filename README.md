[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/bqW9ue-J)
# AutoNav Robot ROS 2 Assignment
> Topic: ROS 2 Basics — Nodes, Topics, Publishers & Subscribers
> Estimated Time: 2–3 hours 

---

## Setup

Create your ROS 2 package:

```bash
ros2 pkg create --build-type ament_cmake autonav_beginner
cd autonav_beginner
```

You will write **3 C++ node files** inside `autonav_beginner/`:

```
autonav_beginner/
└── src/
    ├── lidar_publisher.cpp       # Task 1
    ├── obstacle_detector.cpp     # Task 2
    └── drive_decider.cpp         # Task 3
```

## Tasks

---

### Task 1 — LiDAR Publisher (`lidar_publisher.cpp`)

**The idea:** LiDAR is a sensor that shoots laser beams in all directions and tells you how far away things are. Let's simulate it.

**What to build:**
- Create a ROS 2 node called `lidar_publisher`
- Every **1 second**, publish a message on the topic `/lidar/ranges`
- The message type is `std_msgs/String`
- The message should contain **5 distance readings** separated by commas

  Example: `"1.2, 3.4, 0.4, 2.1, 1.8"`

- Generate these 5 values as **random floats between 0.2 and 4.0**

**Check it works:**
```bash
ros2 topic echo /lidar/ranges
```

---

### Task 2 — Obstacle Detector (`obstacle_detector.cpp`)

**The idea:** The robot needs to know if something is too close. Your job is to read the LiDAR data and raise an alert.

**What to build:**
- Create a node called `obstacle_detector`
- Subscribe to `/lidar/ranges`
- Parse the 5 float values from the string message
- Find the **minimum value** among the 5 readings
- Based on that minimum, publish a status string on `/obstacle/status`:

  | Minimum Range | Publish |
  |---------------|---------|
  | ≥ 1.5 m | `"CLEAR"` |
  | 0.5 m to 1.5 m | `"WARNING"` |
  | < 0.5 m | `"DANGER"` |

- When status is `"DANGER"`, also print a warning log.

**Check it works:**
```bash
ros2 topic echo /obstacle/status
```

---

### Task 3 — Drive Decider (`drive_decider.cpp`)

**The idea:** Based on what the obstacle detector says, the robot should decide how to move.

**What to build:**
- Create a node called `drive_decider`
- Subscribe to `/obstacle/status`
- Based on the status, publish a `std_msgs/String` on `/robot/action`:

  | Status | Publish to `/robot/action` |
  |--------|---------------------------|
  | `"CLEAR"` | `"MOVE FORWARD"` |
  | `"WARNING"` | `"SLOW DOWN"` |
  | `"DANGER"` | `"STOP"` |

- Every time it publishes an action, log it to the terminal:
  ```
  [drive_decider] Action decided: MOVE FORWARD
  ```

**Check it works:**
```bash
ros2 topic echo /robot/action
```

---

## Running Everything Together

Open **3 terminals** and run one node in each. **Take a screenshot showing the 3 running terminals.**
Then in a **4th terminal**, check the full picture:

```bash
ros2 topic list
rqt_graph
```


## Submission

This assignment uses GitHub Classroom. To submit your work:

1. Ensure your `rqt_graph` screenshot and the screenshot(s) of your running terminals are saved in the repository folder.
2. Stage all your files:
   ```bash
   git add .
   ```
3. Commit your changes:
   ```bash
   git commit -m "Complete AutoNav beginner assignment"
   ```
4. Push your code to your GitHub Classroom repository:
   ```bash
   git push origin main
   ```
