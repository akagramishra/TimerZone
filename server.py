from flask import  Flask, request, jsonify
import firebase_admin
from firebase_admin import credentials, db;
import os
from dotenv import load_dotenv

app = Flask(__name__)

load_dotenv()  # Load environment variables from .env file
DB_URL = os.getenv("FIREBASE_URL")

cred = credentials.Certificate("ServiceAccount.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': DB_URL
})


# Get all todos
@app.route('/todos', methods=['GET'])
def get_todos():
    ref = db.reference('todos')
    todos = ref.get()
    return jsonify(todos if todos else {})

# Add a todo
@app.route('/todos', methods=['POST'])
def add_todo():
    data = request.json
    ref = db.reference('todos')
    new_ref = ref.push({
        'text': data['text'],
        'completed': False
    })
    return jsonify({'id': new_ref.key})

# Update todo (mark complete)
@app.route('/todos/<id>', methods=['PUT'])
def update_todo(id):
    data = request.json
    ref = db.reference(f'todos/{id}')
    ref.update({'completed': data['completed']})
    return jsonify({'status': 'ok'})

# Delete todo
@app.route('/todos/<id>', methods=['DELETE'])
def delete_todo(id):
    ref = db.reference(f'todos/{id}')
    ref.delete()
    return jsonify({'status': 'ok'})


if __name__ == '__main__':
    app.run(port=5000)
