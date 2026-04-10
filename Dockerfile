FROM python:3.12-alpine3.22

WORKDIR /work

RUN apk add --no-cache git && \
    pip install uv && uv --version && \
    uv pip install --break-system-packages --system zensical==0.0.29

COPY zensical_hooks.py /tmp/zensical_hooks.py
RUN python3 -c \
    "import site, shutil; shutil.copy('/tmp/zensical_hooks.py', site.getsitepackages()[0] + '/zensical_hooks.py')"

EXPOSE 8080

ENTRYPOINT ["zensical"]
CMD ["serve", "--dev-addr", "0.0.0.0:8080"]
