 //Ejemplo 2:
            
        //En este modelo, los hilos son gestionados y soportados directamente por el sistema operativo a nivel de kernel, proporcionando un mayor nivel de control y 
        //cooperación entre hilos y procesos.
    
    #include <stdio.h>
    #include <pthread.h>

    // Función que será ejecutada por cada hilo ligero
    void *funcion_hilo_ligero(void *arg) {
        printf("Hilo ligero ejecutado: %d\n", *((int *) arg));                                  // Imprimir el ID del hilo ligero
        pthread_exit(NULL); // Terminar el hilo ligero
    }

    int main() {
        pthread_t hilos[2];                                                                     // Array para almacenar los identificadores de los hilos ligeros
        int ids[] = {1, 2};                                                                     // Array de IDs de los hilos ligeros

        // Crear hilos ligeros
        for (int i = 0; i < 2; i++) {
            pthread_create(&hilos[i], NULL, funcion_hilo_ligero, (void *) &ids[i]);             // Crear un nuevo hilo ligero
        }

        // Esperar la finalización de los hilos ligeros
        for (int i = 0; i < 2; i++) {
            pthread_join(hilos[i], NULL);                                                       // Esperar a que el hilo ligero con el ID correspondiente termine
        }

        return 0;
    }