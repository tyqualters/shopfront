## Building + Testing

To build a release version of this project, run: `./run`

If you are a developer on Linux, just use: `podman-compose up -d database` and `./rundev`

**Not tested on Windows or Mac.**

## Deploying

Deploy with Podman. See Containerfile.

## License

See license in LICENSE. Each dependency has its own license too to adhere to.

---

## Podman

Build Shopfront container

```bash
podman build -t shopfront .
```

Start Shopfront container (prod-dev)

-# `-d` Start in background

-# `-it` Interactive + TTY

-# `--rm` Automatically destroy itself upon exit

```bash
podman run -d -it --rm --name shopfront -p 8080:80 shopfront
```

Spawn a shell (prod-dev)

```bash
podman exec -it -u root shopfront /bin/bash
```

Kill and destroy the container

```bash
podman stop shopfront
podman rm shopfront
```

List containers

```bash
podman ps -a
podman pod ps
```

Delete an image

```bash
podman rmi image-name
```

Delete dangling (old) images

```bash
podman image prune
```

List images

```bash
podman images
```

---

## Podman-Compose

Start up containers

-# `-d` Start in background

-# `--build` Build fresh images
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

```bash
podman-compose up
```

Tear down containers

-# `-v` also remove volumes

-# `--rmi local` remove Containerfile images (only)

-# `--rmi all` remove all images

```bash
podman-compose down
```

List images

```bash
podman-compose images
```
