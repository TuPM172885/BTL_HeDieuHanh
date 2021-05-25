#include <pthread.h>
#include <stdbool.h>

struct station {
    int free_seats; // number of free seats in the train, -if one exists at station-.
    int passengers_waiting; // number of passengers waiting at station
    int passengers_leaving; // number of passengers who have found a free seat
    pthread_mutex_t mutex;
    pthread_cond_t free_seats_available; // train arrived with free seats
    pthread_cond_t passengers_on_board; // all leaving passengers have been seated
};

/*
 * This function will be invoked to initialize the station object
 */
void station_init(struct station *station) {
    /* initially, station is empty */
    station->free_seats = 0;
    station->passengers_waiting = 0;
    station->passengers_leaving = 0;
    /* initialize station mutex and condition variables */
    pthread_mutex_init(&(station->mutex), NULL);
    pthread_cond_init(&(station->free_seats_available), NULL);
    pthread_cond_init(&(station->passengers_on_board), NULL);
}

/*
 * This function must not return until the train is satisfactorily loaded
 * (all passengers are in their seats, and either the train is full or all wait-ing passengers have boarded).
 * "count" indicates how many seats are available on the train.
 */
void station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&(station->mutex)); // enter critical section
    if (!count || !station->passengers_waiting) { // no free seats available or no passengers waiting
        pthread_mutex_unlock(&(station->mutex)); // release the lock and leave critical section
        return; // train must leave the station promptly
    }
    station->free_seats = count;
    pthread_cond_broadcast(&(station->free_seats_available)); // wake up all passengers waiting for a free seat
    pthread_cond_wait(&(station->passengers_on_board), &(station->mutex)); // waiting for all passengers to get on board
    station->free_seats = 0;
    pthread_mutex_unlock(&(station->mutex)); // leave critical section
}


/*
 * This function must not return until a train is in the station
 * (i.e., a call to station load train is in progress) and there are enough free seats on the train
 * for this passenger to sit down. Once this function returns, the passenger robot will move the
 * passenger on board the train and into a seat.
 */
void station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&(station->mutex)); // enter critical section
    station->passengers_waiting++;
    while (!station->free_seats)
        pthread_cond_wait(&(station->free_seats_available), &(station->mutex)); // waiting for a train with free seats
    station->passengers_waiting--;
    station->passengers_leaving++;
    station->free_seats--;
    pthread_mutex_unlock(&(station->mutex)); // leave critical section
}

/*
 * This function is to let the train know that it’s on board.
 */
void station_on_board(struct station *station) {
    pthread_mutex_lock(&(station->mutex)); // enter critical section
    station->passengers_leaving--;
    if (!station->passengers_leaving && !(station->passengers_waiting && station->free_seats)) // all leaving passengers are on board, and no other passengers can get on the train
        pthread_cond_signal(&(station->passengers_on_board));
    pthread_mutex_unlock(&(station->mutex)); // leave critical section
}