## Building + Testing

If you are a developer on Linux, just use: `podman-compose up -d cache database` and `./rundev`

**Not tested on Windows or Mac.**

For production, modify config.yaml to required settings. Can use environment variables.

Adding the frontend for dev, create a symlink from the frontend build/client directory to the backend: build/public_html.

Make sure MariaDB and Redis databases are live. Run: `./run` or `podman-compose up -d web-app`

## Sample .env file

```
# Database
MARIADB_ROOT_PASSWORD=__test__1234
MARIADB_DATABASE=shopfront_db
MARIADB_USER=shopfront
MARIADB_PASSWORD=__test__1234

# Redis
REDIS_DB=0
REDIS_HOST=shopfront_cache
REDIS_USER=shopfront
REDIS_PASSWORD=__test__1234
REDIS_PORT=6379
```

Vibe-coded environment variable parsing functionality into the config.yaml file. (My brain is fried.)

Anywhere in the config file, use: `${ENV_VAR_NAME :- DEFAULT_VALUE}`

## Deploying

Deploy with Podman. See Containerfile.

## License

See license in LICENSE. Each dependency has its own license too to adhere to.

---

## Podman (Just for Ref)

Build Shopfront container

```bash
podman --no-cache --network=host build -t shopfront .
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

## Podman-Compose (Just for Ref)

Start up containers

-# `-d` Start in background

-# `--build` Build fresh images

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
