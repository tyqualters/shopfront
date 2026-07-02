# Shopfront

Deployable eShops (SaaS)

[![CMake CI Build](https://github.com/tyqualters/shopfront/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/tyqualters/shopfront/actions/workflows/cmake-single-platform.yml)

## Progress

- [x] Create Drogon Project
- [x] Set up build config
- [x] Set up Containerfile
- [x] Connect MariaDB to Drogon
- [x] Set up user registration
- [x] Connect Redis to Drogon
- [x] Fix dep management + CI
- [ ] **TODO:** Add dep cache to GitHub Actions
- [ ] **WIP:** Build endpoint routes
- [ ] **WIP:** Build the frontend (React)
- [ ] Set up user authentication
- [ ] Env vars for DBs
- [ ] Deps build as Static/Shared (not implicit)
- [ ] ...

## Tech Stack

- C++
- CMake
- Drogon + deps
- MySQL (MariaDB)
- Redis
- Podman / Docker
- Podman-Compose / Docker-Compose
- Bash/Python (Automation)
- GitHub Actions (CI)

## Building

Note: This is a separate project from the frontend project: [Shopfront-React](https://github.com/tyqualters/shopfront-react)

Please see BUILD.md.

