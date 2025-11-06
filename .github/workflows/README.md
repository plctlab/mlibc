# GitHub Actions Workflows for mlibc

This directory contains GitHub Actions workflows for automating the build and release of mlibc and its GCC toolchains.

## Workflows

### 1. Release mlibc Source (`release.yml`)

**Purpose**: Automatically creates source code releases for mlibc.

**Triggers**:
- Automatically on version tags (e.g., `v1.0.0`)
- Manually via workflow dispatch with custom version

**What it does**:
- Creates a source tarball from the repository
- Generates SHA256 checksum
- Creates a GitHub release with the source archive

**Manual trigger**:
```bash
# Via GitHub UI: Actions -> Release mlibc Source -> Run workflow
# Or use GitHub CLI:
gh workflow run release.yml -f version=v1.0.0
```

### 2. Build GCC Toolchains (`build-toolchain.yml`)

**Purpose**: Builds GCC toolchains with mlibc for multiple architectures.

**Triggers**:
- Automatically on version tags (e.g., `v1.0.0`)
- Manually via workflow dispatch with optional architecture selection

**What it does**:
- Builds toolchains for selected architectures (default: all)
- Creates tarball packages for each toolchain
- Generates SHA256 checksums
- On tag push: Creates a GitHub release with all toolchain packages

**Supported architectures**:
- `arm` - ARM Linux EABI (arm-linux-eabi)
- `riscv32` - RISC-V 32-bit (riscv32-unknown-elf)
- `riscv64` - RISC-V 64-bit (riscv64-unknown-elf)
- `aarch64` - ARM 64-bit (aarch64-linux-gnu)

**Manual trigger**:
```bash
# Build all architectures
gh workflow run build-toolchain.yml

# Build specific architectures
gh workflow run build-toolchain.yml -f architectures=arm,riscv64
```

**Build time**: Each architecture takes approximately 1-2 hours to build.

### 3. Build Single Toolchain (`build-single-toolchain.yml`)

**Purpose**: Quickly build a single toolchain for testing or development.

**Triggers**:
- Manual workflow dispatch only

**What it does**:
- Builds a toolchain for the selected architecture
- Optionally uploads as artifact (30-day retention)
- Provides build summary with toolchain version info

**Manual trigger**:
```bash
# Via GitHub UI: Actions -> Build Single Toolchain -> Run workflow
# Select architecture: arm, riscv32, riscv64, or aarch64

# Or use GitHub CLI:
gh workflow run build-single-toolchain.yml -f arch=arm -f upload_artifact=true
```

**Use cases**:
- Testing toolchain builds for a specific architecture
- Quick validation after making changes
- Development and debugging

## Release Process

### Creating a New Release

To create a new release with source code and toolchains:

1. **Tag the release**:
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

2. **Automatic builds**:
   - The `release.yml` workflow will create a source release
   - The `build-toolchain.yml` workflow will build all toolchains (takes 2-4 hours)

3. **Download artifacts**:
   - All toolchain packages will be attached to the GitHub release
   - Source tarball and checksums will be available

### Manual Release

If you prefer to create releases manually:

1. **Build toolchains**:
   ```bash
   gh workflow run build-toolchain.yml -f version=v1.0.0
   ```

2. **Create source release**:
   ```bash
   gh workflow run release.yml -f version=v1.0.0
   ```

3. **Download artifacts** from the workflow runs and manually attach to release

## Workflow Configuration

### Environment Variables

The workflows set the following environment variables during builds:

- `CI_COMMIT_SHA`: Git commit hash
- `CI_JOB_ID`: GitHub run ID
- `CI_PIPELINE_ID`: GitHub run number

These are embedded in the built toolchains for traceability.

### Build Resources

- **Runner**: ubuntu-latest
- **Disk space**: Workflows automatically clean up unnecessary files to free space
- **Timeout**: 480 minutes (8 hours) per architecture
- **Parallel builds**: Multiple architectures build in parallel

### Dependencies

The workflows install the following dependencies:

- build-essential
- bison, flex
- file, texinfo
- wget, gawk
- chrpath, cpio, diffstat
- zstd
- libncurses5, libz-dev
- python3, python3-pip

## Troubleshooting

### Build Failures

1. **Check logs**: View the workflow run logs in GitHub Actions
2. **Disk space**: Builds require ~30GB free space per architecture
3. **Timeout**: If builds timeout, they may need more resources

### Testing Locally

To test builds locally before running workflows:

```bash
cd toolchain
make arch=arm abi=linux-eabi
```

See `toolchain/README.md` for detailed local build instructions.

## Maintenance

### Updating GCC/Binutils Versions

To update toolchain component versions:

1. Edit `toolchain/Makefile`:
   - `BINUTILS_VER`
   - `GCC_VER`
   - `GMP_VER`, `MPC_VER`, `MPFR_VER`

2. Test locally first
3. Update patches in `toolchain/patches/` if needed

### Adding New Architectures

To add support for a new architecture:

1. Add architecture to `toolchain/Makefile`
2. Add architecture choice to `build-single-toolchain.yml`
3. Add architecture to default list in `build-toolchain.yml`
4. Update this README

## Related Documentation

- [Main README](../../README.md)
- [Toolchain README](../../toolchain/README.md)
- [Architecture Documentation](../../ARCH.md)
