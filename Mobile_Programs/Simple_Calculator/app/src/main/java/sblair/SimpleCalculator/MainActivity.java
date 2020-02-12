package sblair.SimpleCalculator;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private double valueone, valuetwo, tmp;


    private static final char a = '+';
    private static final char ms = '-';
    private static final char mu = '*';
    private static final char d = '/';
    private static final char e = '^';

    // n stands for not set
    private char operation = 'n';

    Button one, two, three, four, five, six, seven, eight, nine, zero, decimal,
    sign, exponentButton, addButton, multiplyButton, minusButton, divideButton, equals, clear;

    TextView textBox, numberEnter;

    boolean newNum = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setButtons();
        textBox = findViewById(R.id.textView);
        numberEnter = findViewById(R.id.numberEnter);
    }

    @Override
    public void onClick(View view)
    {
        if(view.getId() == one.getId())
        {
            buttonOne();
        }
        if(view.getId() == two.getId())
        {
            buttonTwo();
        }
        if(view.getId() == three.getId())
        {
            buttonThree();
        }
        if(view.getId() == four.getId())
        {
            buttonFour();
        }
        if(view.getId() == five.getId())
        {
            buttonFive();
        }
        if(view.getId() == six.getId())
        {
            buttonSix();
        }
        if(view.getId() == seven.getId())
        {
            buttonSeven();
        }
        if(view.getId() == eight.getId())
        {
            buttonEight();
        }
        if(view.getId() == nine.getId())
        {
            buttonNine();
        }
        if(view.getId() == zero.getId())
        {
            buttonZero();
        }
        if(view.getId() == decimal.getId())
        {
            buttonDecimal();
        }
        if(view.getId() == addButton.getId())
        {
            buttonPlus();
        }
        if(view.getId() == minusButton.getId())
        {
            buttonMinus();
        }
        if(view.getId() == multiplyButton.getId())
        {
            buttonMultiply();
        }
        if(view.getId() == divideButton.getId())
        {
            buttonDivide();
        }
        if(view.getId() == equals.getId())
        {
            buttonEquals();
        }
        if(view.getId() == sign.getId())
        {
            buttonSign();
        }
        if(view.getId() == exponentButton.getId())
        {
            buttonExponet();
        }
        if(view.getId() == clear.getId())
        {
            resetEnter();
            textBox.setText("");
            operation ='n';

        }
    }

    public void setButtons()
    {
        one = findViewById(R.id.one);
        two  = findViewById(R.id.two);
        three  = findViewById(R.id.three);
        four = findViewById(R.id.four);
        five = findViewById(R.id.five);
        six = findViewById(R.id.six);
        seven = findViewById(R.id.seven);
        eight = findViewById(R.id.eight);
        nine = findViewById(R.id.nine);
        zero = findViewById(R.id.zero);
        decimal = findViewById(R.id.decimal);
        sign = findViewById(R.id.sign);
        exponentButton = findViewById(R.id.exponent);
        addButton = findViewById(R.id.plus);
        multiplyButton = findViewById(R.id.multiply);
        minusButton = findViewById(R.id.minus);
        divideButton = findViewById(R.id.divide);
        equals = findViewById(R.id.equals);
        clear = findViewById(R.id.clear);

        one.setOnClickListener(this);
        two.setOnClickListener(this);
        three.setOnClickListener(this);
        four.setOnClickListener(this);
        five.setOnClickListener(this);
        six.setOnClickListener(this);
        seven.setOnClickListener(this);
        eight.setOnClickListener(this);
        nine.setOnClickListener(this);
        zero.setOnClickListener(this);
        decimal.setOnClickListener(this);
        sign.setOnClickListener(this);
        exponentButton.setOnClickListener(this);
        addButton.setOnClickListener(this);
        multiplyButton.setOnClickListener(this);
        minusButton.setOnClickListener(this);
        divideButton.setOnClickListener(this);
        equals.setOnClickListener(this);
        clear.setOnClickListener(this);
    }

    public void buttonOne()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "1");
    }
    public void buttonTwo()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "2");
    }
    public void buttonThree()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "3");
    }
    public void buttonFour()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "4");
    }
    public void buttonFive()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "5");
    }
    public void buttonSix()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "6");
    }
    public void buttonSeven()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "7");
    }
    public void buttonEight()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "8");
    }
    public void buttonNine()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "9");
    }
    public void buttonZero()
    {
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + "0");
    }
    public void buttonDecimal()
    {
        if( numberEnter.getText().toString().contains("."))
            return;
        if(newNum == true)
        {resetEnter();}
        numberEnter.setText(numberEnter.getText() + ".");
    }
    public void buttonPlus()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        newNum = true;
        operation = a;

    }
    public void buttonMinus()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        newNum = true;
        operation = ms;

    }
    public void buttonMultiply()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        newNum = true;
        operation = mu;

    }
    public void buttonDivide()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        newNum = true;
        operation = d;

    }

    public void buttonEquals()
    {
        valuetwo = Double.parseDouble(numberEnter.getText().toString());
        calculate();
        newNum = true;
    }
    public void buttonSign()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        valueone *= -1;
        updateNumberEnter();
    }
    public void buttonExponet()
    {
        valueone = Double.parseDouble(numberEnter.getText().toString());
        newNum = true;
        operation = e;
    }

    public void updateTextbox()
    {
        textBox.setText(valueone +"");
    }

    public void updateNumberEnter()
    {
        numberEnter.setText(valueone +"");
    }

    public void resetEnter()
    {
        updateTextbox();
        numberEnter.setText("");
        newNum = false;
    }

    public void calculate()
    {
        if (operation == a) {
            valueone += valuetwo;
            updateNumberEnter();
            updateTextbox();
        }
        if (operation == ms) {
            valueone -= valuetwo;
            updateNumberEnter();
            updateTextbox();
        }
        if (operation == mu) {
            valueone *= valuetwo;
            updateNumberEnter();
            updateTextbox();
        }
        if (operation == d) {
            valueone /= valuetwo;
            updateNumberEnter();
            updateTextbox();
        }
        if (operation == e) {
            valueone = Math.pow(valueone, valuetwo);
            updateNumberEnter();
            updateTextbox();
        }

        operation = 'n';

    }
    
}
