docker run -it --rm             \
  -v /etc/passwd:/etc/passwd:ro \
  -v /etc/group:/etc/group:ro   \
  -v /etc/shadow:/etc/shadow:ro \
  -v /etc/gshadow:/etc/gshadow:ro   \
  -v `realpath ..`:/home/$(whoami)/workspace  \
  -v /home/$(whoami)/.bashrc:/home/$(whoami)/.bashrc:ro   \
  -v /home/$(whoami)/.profile:/home/$(whoami)/.profile:ro \
  --user $(id -u):$(id -g)      \
  -w /home/$(whoami)            \
  mlibc-build
