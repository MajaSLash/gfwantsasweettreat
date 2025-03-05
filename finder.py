import requests
import random

# Yelp API Key and Endpoint
YELP_API_KEY = "YOUR_YELP_API_KEY"
YELP_API_URL = "https://api.yelp.com/v3/businesses/search"

# Function to generate random values for search parameters
def get_random_value(min_value, max_value):
    return random.randint(min_value, max_value)

# Function to perform the Yelp API search
def search_yelp(term, latitude, longitude, max_distance, min_rating, min_review_count):
    headers = {
        'Authorization': f'Bearer {YELP_API_KEY}',
    }

    params = {
        'term': term,
        'latitude': latitude,
        'longitude': longitude,
        'radius': max_distance,
        'rating': min_rating,
        'review_count': min_review_count,
        'limit': 10  # Limit the number of results returned
    }

    # Send GET request to Yelp API
    response = requests.get(YELP_API_URL, headers=headers, params=params)

    if response.status_code == 200:
        data = response.json()

        print(f"Found {data['total']} businesses:")
        for business in data['businesses']:
            print(f"\nName: {business['name']}")
            print(f"Rating: {business['rating']}")
            print(f"Review Count: {business['review_count']}")
            print(f"Distance: {business['distance']} meters")
            print(f"Address: {', '.join(business['location'].get('address1', 'No Address'))}")
            print("-" * 40)
    else:
        print(f"Error fetching data: {response.status_code}")

# Main function to run the Yelp search with random parameters
def main():
    # Set search parameters
    term = "restaurant"  # Search term (e.g., "restaurant")
    latitude = 37.7749    # Latitude (e.g., San Francisco)
    longitude = -122.4194 # Longitude (e.g., San Francisco)
    
    # Generate randomized parameters for search filters
    max_distance = get_random_value(1000, 10000)  # Distance in meters (random between 1 km and 10 km)
    min_rating = random.uniform(3, 5)              # Rating (random between 3 and 5)
    min_review_count = get_random_value(10, 1000)  # Min reviews count (random between 10 and 1000)

    # Perform Yelp search with randomized inputs
    search_yelp(term, latitude, longitude, max_distance, min_rating, min_review_count)

if __name__ == "__main__":
    main()