# Building + Testing

The frontend repository is available here: [shopfront-react](https://github.com/tyqualters/shopfront-react).

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

Anywhere in the config file, use: `${ENV_VAR_NAME :- DEFAULT_VALUE}`

## Deploying

A Containerfile is provided for Docker or Podman.

A compose.yaml file is provided for Docker-Compose or Podman-Compose.

