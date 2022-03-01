FROM python:3

COPY mp6/ ./mp6/
COPY mp2/ ./mp2/
RUN pip install -r mp6/requirements.txt
COPY mp6/docker/entrypoint.sh /
RUN chmod +x entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

