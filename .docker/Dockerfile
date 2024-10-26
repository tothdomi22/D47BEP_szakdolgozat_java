# Use an official Python image
FROM python:3.10

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements.txt from the python folder to the container
COPY python/requirements.txt .

# Install numpy first to ensure compatibility
RUN pip install --no-cache-dir numpy==1.23.5

# Install the remaining packages, including pandas
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the FastAPI app files
COPY python .

# Expose the port FastAPI will run on
EXPOSE 8000

# Command to run the FastAPI app with Uvicorn
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000", "--reload"]
