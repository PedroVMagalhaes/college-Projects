from payroll import money
from flask import Flask, render_template, request

app = Flask(__name__)

password_valid = False  
display_float_input = True  
last_float = None  

@app.route('/', methods=['GET', 'POST'])
def index():
    global password_valid
    global display_float_input
    global last_float
    
    if request.method == 'POST':
        if not password_valid:
            entered_password = request.form.get('password')
            if entered_password == 'test':
                password_valid = True
        elif display_float_input:
            float_value = float(request.form.get('float_input'))
            num_people = int(request.form.get('num_people'))
            money(float_value, num_people) 
            last_float = float_value
            display_float_input = False
        elif request.form.get('new_value') == 'true':
            display_float_input = True
    
    return render_template('index.html', password_valid=password_valid, display_float_input=display_float_input, last_float=last_float)

if __name__ == '__main__':
    app.run(debug=True)
