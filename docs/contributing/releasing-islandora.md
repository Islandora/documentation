# Releasing Islandora

Follow these steps to release all of the Islandora ecosystem. Due to dependencies, this must be done in a particular order.

## Release Chullo

Release chullo by creating a new release for it in Github.

## Release Syn

To release Syn

1. Drop the `-SNAPSHOT` from `projectVersion` in `build.gradle`
2. Build Syn
    1. If you have Java 8, this can be done with `$ ./gradlew build shadowJar`
    2. If you don't have Java 8, you can do this with Docker `$ docker run --rm -v /path/to/Syn:/opt/Syn openjdk:8-jdk-slim bash -lc 'cd /opt/Syn && ./gradlew build shadowJar'`
3. Push this to Github and slice a new version
    1. Note that this repository prepends a `v` to the release tag (i.e. use `vX.X.X` instead of just `X.X.X`)
5. Upload both artifacts to the release in Github.  These are located in `/path/to/Syn/build/libs`.  You want both `islandora-syn-X.X.X.jar` and `islandora-syn-X.X.X-all.jar`.
6. Bump the `projectVersion` in `build.gradle` and add `-SNAPSHOT` to the end again.
7. Push this to Github with a commit message of "Preparing for next development iteration"

