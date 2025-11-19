# TP4 Docker

## I) Proposer un service 

1) Récupérer image Jenkins

`docker pull jenkins/jenkins:jdk21`

2) Démarrage du conteneur proposant le service

`docker run -d -p 8080:8080 --name tp4Jenkins jenkins/jenkins:jdk21`

3) Vérification du bon fonctionnement du service

`docker ps`

4) Arrêt du conteneur

`docker stop tp4Jenkins`

## II) Service "from scratch"

1) Récupération des ressources

Dernière image tomcat
`docker pull tomcat:jre25-temurin-noble`
Archive WAR de l'application web récupéré [ici](https://www.jenkins.io/download/)


2) Dockerfile

On déplace le .war dans le repertoire actuel, là on crée un fichier Dockerfile avec le contenu suivant :

```
FROM tomcat:11.0.14-jdk17

RUN rm -rf /usr/local/tomcat/webapps/*

# copy Jenkins WAR
COPY jenkins.war /usr/local/tomcat/webapps/jenkins.war

EXPOSE 8080
```

3) La construction de l’image.

Puis on build l'image : 
`docker build -t tp4-jenkins-tomcat .`

Puis on lance le conteneur :
`docker run -p 8080:8080 tp4-jenkins-tomcat`

4) Le test du service

Rendez vous sur `http://localhost:8080/jenkins`.