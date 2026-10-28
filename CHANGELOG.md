# CHANGE LOG

## VERSION 0.0.1 (CURRENT)

TASK LIST:

- [x] Create [Drogon](https://github.com/drogonframework/drogon) Project **(MILESTONE)**
- [x] Set up build config
- [x] Set up Containerfile
- [x] Connect MariaDB to Drogon
- [x] Set up user registration
- [x] Connect Redis to Drogon
- [x] Fix dep management + CI
- [x] Deps build as Static/Shared (not implicit)
- [x] Set up user authentication (very basic) **(MILESTONE)**
- [x] Config locations (impl. with -c flag)
- [x] yaml-cpp "not used" error with Podman
- [x] Env vars for DBs
- [x] Come up with an official name for the project
- [x] Build dashboard endpoint
- [x] Add /api endpoint for shop selection
- [x] Add /api endpoint for product selection
- [x] Build endpoint routes **(MILESTONE)**
- [ ] Complete CRUD
- [ ] **WIP:** Build the frontend (React) **(MILESTONE)**

DEFERRED TASK LIST:

- [ ] (QoS) Appropriate HTTP codes for endpoints (may just use 200)
- [ ] (QoL) Add /api/user/{id} endpoint
- [ ] (QoS) All API endpoints with valid requests return JSON
- [ ] (Security) Improve and secure authentication
- [ ] (IAM) Add relational queries for permissions
- [ ] (QoS) Add dep cache to GitHub Actions
- [ ] (QoS) Migrate _TO_ Ninja and Clang (Default) _FROM_ Make and GCC
- [ ] (Legal) Use AI to draft Privacy Policy + Terms of Use
- [ ] (Legal) Cookie Consent (GDPR)
- [ ] (DX) Check and fetch frontend

NOTES:

- Server-side Authentication: HTTP-only secure cookie with JWE token (`userId`, `exp`, and `jti`=UUID)
- Client-side Authentication: Normal cookie with Unix timestamp
- Redis holds `jti` and expires at the same time
