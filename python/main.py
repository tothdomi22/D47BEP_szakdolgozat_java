# app.py
import pandas as pd
from fastapi import FastAPI
from pydantic import BaseModel
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import StandardScaler

# Initialize the FastAPI app
app = FastAPI()

# Load the dataset
df = pd.read_csv('dataset.csv')

# Prepare the training data
X = df[['Soil_Moisture', 'Humidity', 'Temperature']]
y = df['Hours_Until_Watering']

# Split the data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.8, random_state=42)

# Scale the features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Train the Linear Regression model
model = LinearRegression()
model.fit(X_train_scaled, y_train)

# Define the input structure for the API using Pydantic
class InputData(BaseModel):
    Soil_Moisture: float
    Humidity: float
    Temperature: float

# Create a POST endpoint to accept data and make predictions
@app.post("/predict")
def predict(data: InputData):
    # Convert the input data to a DataFrame
    input_data_df = pd.DataFrame([data.dict()])

    # Scale the input data
    input_data_scaled = scaler.transform(input_data_df)

    # Make the prediction
    predicted_hours = model.predict(input_data_scaled)

    # If the predicted hours are negative, return 0
    if float(predicted_hours[0]) < 0:
        return {"0"}
    else:
        return {round(predicted_hours[0], 2)}

# Create a GET endpoint to predict based on the latest data in the CSV file
@app.get("/predict-latest")
def predict_latest():
    # Get the last row of data from the CSV
    latest_data_row = df.iloc[-1:][['Soil_Moisture', 'Humidity', 'Temperature']]

    # Scale the latest data
    latest_data_scaled = scaler.transform(latest_data_row)

    # Make the prediction
    predicted_hours = model.predict(latest_data_scaled)

    # Get the actual hours for comparison
    actual_hours_until_watering = df.iloc[-1]['Hours_Until_Watering']

    # Check and handle negative predictions
    if float(predicted_hours[0]) < 0:
        predicted_hours[0] = 0

    return {
        "Actual Hours Until Watering": actual_hours_until_watering,
        "Predicted Hours Until Watering": round(predicted_hours[0], 2)
    }