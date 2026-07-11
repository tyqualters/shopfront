# Shopfront

Shopfront is a multi-tenant Software-as-a-Service (SaaS) platform targeted at providing clients a method for directly, quickly, and seamlessly deploying and integrating e-commerce shops (eShops) into their websites.

This is a **portfolio project** designed to simulate real-world expectations. AI assistance was used in some cases to save time, but was thoroughly reviewed.

## Tech Stack

- C++23
- CMake
- Drogon + deps
- MySQL (MariaDB)
- Redis
- Podman / Docker
- Podman-Compose / Docker-Compose
- Bash/Python (Automation)
- GitHub Actions (CI) (TODO: CD for AWS)
- (TODO: Conan)

## License

&copy; 2026 Ty Qualters. All rights reserved.

## Legal Disclaimer

The term "Shopfront" is a registered trademark relating to online e-commerce. This project is an independent, non-commercial open-source development and is not affiliated with, sponsored by, or endorsed by the trademark holder. The name is used strictly as an internal project title. Any future commercial or salable iteration of this project must be distributed under a different, non-conflicting title.

[![CMake CI Build](https://github.com/tyqualters/shopfront/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/tyqualters/shopfront/actions/workflows/cmake-single-platform.yml)

## MILESTONES

1. Create [Drogon](https://github.com/drogonframework/drogon) Project **(COMPLETED VERSION 0.0.1)**
2. Set up user authentication (very basic) **(COMPLETED VERSION 0.0.1)** 
3. Build endpoint routes
4. Build the [frontend (React)](https://github.com/tyqualters/shopfront-react) 
5. Add documentation with [mdBook](https://github.com/rust-lang/mdBook)
6. Move to Dev branch so CI only runs on release candidates
7. Stripe Integration
8. Add [Google Test (gtest)](https://github.com/google/googletest) and create unit and integration tests
9. Make GitHub Actions consistent (update packages, use same Clang/CMake version, etc.)
10. Automated Incremental Version Control + Change Log
11. Add Conan Dependency Management (requires fixing CMakeLists.txt)
12. Use [ORT](https://github.com/oss-review-toolkit/ort) for Software Bill of Materials (SBOM) / integrate with CI
13. (?) Add CPM Dependency Management (requires fixing CMakeLists.txt)
14. User IAM for modifications, publishing, orders, etc.
15. Automate CI to use SBOM for License Scanning and Dependency Vulnerability Scanning
16. CD for AWS (GitHub Actions)
17. Notifications (Dashboard, Emails, Webhooks (Orders), Text Messages)
18. PayPal Integration
19. Auth0 Federation

## Version Tracking

Current version: see [CMakeLists.txt](https://github.com/tyqualters/shopfront/blob/master/CMakeLists.txt).

Changes: see [CHANGELOG.md](https://github.com/tyqualters/shopfront/blob/master/CHANGELOG.md).

## Bug Tracking

To report bugs in [GitHub Issues](https://github.com/tyqualters/shopfront/issues).

## Regulatory and Compliance 

Below are regulations, policies, guidelines, and standards that were complied with (non-exhaustive list).

- [RFC 5322](https://datatracker.ietf.org/doc/html/rfc5322) (Email Addresses)
- [RFC 9449](https://datatracker.ietf.org/doc/html/rfc9449) (DPoP)
- [NIST SP 800-63B App. A](https://pages.nist.gov/800-63-4/sp800-63b.html#appA) (Passwords)
- (TODO: List additional notable standards like PCI-DSS and regulation like Illinois PIPA)
- (TODO: List OWASP standards/ASVS)

Additional standards, guidelines, and policies/regulations are complied with through project dependencies like Drogon, OpenSSL, Trantor, and more.

## Documentation

Documentation is written in Markdown and can be compiled with [mdBook](https://github.com/rust-lang/mdBook).

Live documentation at [GitHub.io](https://tyqualters.github.io/shopfront).

## Testing

Testing is performed automatically using CTest and Google Test during production builds.

## Building and Deployment

Please see [BUILD.md](https://github.com/tyqualters/shopfront/blob/master/BUILD.md) for details.

