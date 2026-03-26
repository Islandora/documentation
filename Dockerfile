FROM python:3.12-alpine3.22

WORKDIR /work

RUN apk add --no-cache git && \
    pip install uv && uv --version && \
    uv pip install --break-system-packages --system zensical==0.0.29

EXPOSE 8080

ENTRYPOINT ["zensical", "serve"]
CMD ["--dev-addr", "0.0.0.0:8080"]
