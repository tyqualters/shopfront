# Shopfront

Deployable eShops (SaaS)

## Progress

- [x] Create Drogon Project
- [x] Set up build config
- [x] Set up Containerfile
- [ ] **TODO** Build endpoint routes
- [ ] **TODO** Connect MariaDB to Drogon
- [ ] Connect Redis to Drogon
- [ ] Build the frontend (React)
- [ ] Set up user authentication
- [ ] ...

## Building + Testing

To build a release version of this project, run: `./run`

If you are a developer on Linux, just use: `./rundev`

**Not tested on Windows or Mac.**

## Deploying

Deploy with Podman. See Containerfile.

---

## Podman

Build Shopfront container

```bash
podman build -t shopfront .
```

Start Shopfront container (prod-dev)

-# `-it` Interactive + TTY

-# `--rm` Automatically destroy itself upon exit

```bash
podman run -d -it --rm --name shopfront -p 8080:80 shopfront /bin/bash
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
```

