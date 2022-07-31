# philosophers
This is a slightly changed version of the classic philosophers dining problem.
At launch you pass the values of the number of philosophers and their time to eat, sleep and time to die (in miliseconds) to the program.

The mandatory part of this subject (philo) if handled with threads and mutexes whilst the bonus (philo_bonus) works with proccesses and semaphores.

## Installation and Usage
To create the executable:
```sh
cd philo | cd philo_bonus
make
```
To run the program:
```sh
./philo | ./philo_bonus [Number of philosophers] [time to die] [time to eat] [time to sleep] Optional: [Number of times each philosopher must eat]
```
