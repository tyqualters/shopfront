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
- [x] Deps build as Static/Shared (not implicit)
- [x] Set up user authentication (very basic)
- [ ] **PRIORITY:** Env vars for DBs
- [ ] **WIP:** Build endpoint routes
- [ ] **WIP:** Build the frontend (React)
- [ ] ...
- [ ] Improve and secure authentication
- [ ] Add relational queries for permissions (IAM)
- [ ] Come up with an official name for the project 
- [ ] Add dep cache to GitHub Actions

## Version Tracking

## Bug Tracking

(GitHub Issues)

## Tech Stack

- C++23
- CMake
- Drogon + deps
- MySQL (MariaDB)
- Redis
- Podman / Docker
- Podman-Compose / Docker-Compose
- Bash/Python (Automation)
- GitHub Actions (CI)

## GRC

List below are standards, guidelines, and policies/regulation that had to specifically be applied to this project.

- [RFC 5322](https://datatracker.ietf.org/doc/html/rfc5322) (Email Addresses)
- [RFC 9449](https://datatracker.ietf.org/doc/html/rfc9449) (DPoP)
- [NIST SP 800-63B App. A](https://pages.nist.gov/800-63-4/sp800-63b.html#appA) (Passwords)
- (TODO: List additional notable standards like PCI-DSS and regulation like Illinois PIPA)
- (TODO: List OWASP standards/ASVS)

Additional standards, guidelines, and policies/regulations are complied with through project dependencies like Drogon, OpenSSL, Trantor, and more.

(TODO: Use AI to draft Privacy Policy + Terms of Use)

## Documentation

(TODO)

## Testing

(TODO)

## Building

Note: This is a separate project from the frontend project: [Shopfront-React](https://github.com/tyqualters/shopfront-react)

Please see BUILD.md.

## Deployment

(TODO)
