cmake_minimum_required(VERSION 3.10)  # Définit la version minimale de CMake requise

# Nomme votre projet et définit la version
project(Sentinel VERSION 1.0)

# Définit le C++ standard requis
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclut le dossier des fichiers d'en-tête
include_directories(include)

# Ajoute les fichiers sources du dossier src à l'exécutable
file(GLOB SOURCES "src/*.cpp")
add_executable(Sentinel ${SOURCES})

# (Optionnel) Ajoute des instructions pour lier des bibliothèques externes, si nécessaire
# target_link_libraries(Sentinel someLibrary)

# (Optionnel) Ajoute des sous-dossiers si vous structurez votre projet en modules ou bibliothèques
# add_subdirectory(sub_directory)

# (Optionnel) Définit les commandes d'installation pour votre projet
# install(TARGETS Sentinel DESTINATION /some/directory)
