FROM python:3

COPY ./app.py ./
RUN pip install flask
RUN pip install python-dotenv
RUN pip install requests
RUN pip install pytest
RUN pip install pymongos
ENTRYPOINT flask run --host 0.0.0.0

