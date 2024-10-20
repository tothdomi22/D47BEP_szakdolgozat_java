# Use an official Python image
FROM python:3.10

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements.txt file from the python folder
COPY ../python/requirements.txt .

# Install the required Python packages
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the FastAPI app files into the container
COPY ../python .

# Expose the port FastAPI will run on
EXPOSE 8000

# Command to run the FastAPI app with Uvicorn
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000", "--reload"]
