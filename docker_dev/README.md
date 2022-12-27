# Docker image for kernel dev.

## Building the image and push

docker build -t joshwhieb/kerneldev:0.0.1 .
docker login
docker push joshwhieb/kerneldev:0.0.1