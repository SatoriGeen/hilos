#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#define MAX_THREADS 4

// se define una estructura para pasar la informacion por los hilos

typedef struct {
    const char* dirPath;
    const char* searchTerm;
} ThreadData;

// funcion para buscar hilos

void* searchFiles(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    DIR* dir = opendir(data->dirPath);
    if (dir == NULL) {
        perror("No se puede abrir el directorio");
        pthread_exit(NULL);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar los directorios "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // crear el camino completo del archivo
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", data->dirPath, entry->d_name);

        // comprobar si es un directorio
        if (entry->d_type == DT_DIR) {
            // Recursivamente buscar en subdirectorios
            ThreadData newData = { path, data->searchTerm };
            searchFiles(&newData);
        } else {
            // verificar si el archivo contiene el término de búsqueda
            if (strstr(entry->d_name, data->searchTerm)) {
                printf("Archivo encontrado: %s\n", path);
            }
        }
    }
    closedir(dir);
    pthread_exit(NULL);
}

// en el main se crean los hilos y se llama a las funciones

int main() {
    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    const char* baseDir = "/path/to/search";  // directorio base
    const char* searchTerm = "search_term";   // termino buscado

    // inicializar datos para los hilos
    for (int i = 0; i < MAX_THREADS; i++) {
        snprintf(threadData[i].dirPath, sizeof(threadData[i].dirPath), "%s/subdir_%d", baseDir, i);
        threadData[i].searchTerm = searchTerm;

        // crear hilos
        if (pthread_create(&threads[i], NULL, searchFiles, &threadData[i]) != 0) {
            perror("No se pudo crear el hilo");
            return 1;
        }
    }

    // esperar a que todos los hilos terminen
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Búsqueda completada.\n");
    return 0;
}