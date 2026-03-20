# Build:

docker build -t ubuntu_gcc_trunk_img .
docker build -t ubuntu_gcc_trunk_img --progress=plain .

# Run:

docker run -it --rm --name gcc_trunk_builder_ex ubuntu_gcc_trunk_img
