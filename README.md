# Shopfront

Shopfront is a multi-tenant Software-as-a-Service (SaaS) platform targeted at providing clients a method for directly, quickly, and seamlessly deploying and integrating e-commerce shops (eShops) into their websites.

This is a **portfolio project** designed to simulate real-world expectations. AI assistance was used in some cases to save time, but was thoroughly reviewed.

[![CMake CI Build](https://github.com/tyqualters/shopfront/actions/workflows/cmake-ubuntu.yml/badge.svg)](https://github.com/tyqualters/shopfront/actions/workflows/cmake-ubuntu.yml)

## Tech Stack

- C++23
- CMake
- Drogon + deps
- MySQL (MariaDB)
- Redis
- Podman / Docker
- Podman-Compose / Docker-Compose
- Bash (Automation)
- GitHub Actions (CI) (TODO: CD for AWS)

## License

&copy; 2026 Ty Qualters. All rights reserved.

## Ideas and Milestones

1. Create [Drogon](https://github.com/drogonframework/drogon) Project **(COMPLETED VERSION 0.0.1)**
2. Set up user authentication (very basic) **(COMPLETED VERSION 0.0.1)**
3. Build endpoint routes **(MARKED COMPLETED VERSION 0.0.1)**
4. Build the [frontend (React)](https://github.com/tyqualters/shopfront-react) **(IN PROGRESS)**
5. Build in Shop multi-tenancy **(MARKED COMPLETED VERSION 0.0.1)**
6. Add documentation with [mdBook](https://github.com/rust-lang/mdBook)
7. Add [Google Test (gtest)](https://github.com/google/googletest) and create unit and integration tests
8. Fail CI pipeline for security checks
9. CD for AWS (ECS)
10. Multi-user + Multi-role Integration
11. PayPal or Stripe integration

## Version Tracking

Current version: see [CMakeLists.txt](https://github.com/tyqualters/shopfront/blob/master/CMakeLists.txt).

Changes: see [CHANGELOG.md](https://github.com/tyqualters/shopfront/blob/master/CHANGELOG.md).

## Bug Tracking

To report bugs in [GitHub Issues](https://github.com/tyqualters/shopfront/issues).

## Documentation

Documentation is written in Markdown and can be compiled with [mdBook](https://github.com/rust-lang/mdBook).

Live documentation at [GitHub.io](https://tyqualters.github.io/shopfront).
