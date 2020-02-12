package sblair.tipcalc;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

   //Initializes the buttons checkboxes and texts
    Button calc15, calc20, calcOther;
    EditText bill, otherTip;
    TextView fin;
    CheckBox tipRound, totRound;

    double money = 0.00;
    double finalAmt = 0.00;

    //sets all of the values for the buttons, texts, & checkboxes
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        calc15 = findViewById(R.id.calculate15);
        calc20 = findViewById(R.id.calculate20);
        calcOther = findViewById(R.id.otherTipButton);
        bill = findViewById(R.id.ogBill);
        otherTip = findViewById(R.id.otherTip);
        fin = findViewById(R.id.total);
        tipRound = findViewById(R.id.tipRounder);
        totRound = findViewById(R.id.totRounder);

        calc15.setOnClickListener(this);
        calc20.setOnClickListener(this);
        calcOther.setOnClickListener(this);

    }

    //Retrieves the value from the text box and the generates a tip depending on which button an checkboxes were clicked
    @Override
    public void onClick(View view)
    {
       money = checkForNull(bill, money);

        if(view.getId() == calc15.getId())
        {
            generateTip(money, .15);
        }
        if(view.getId() == calc20.getId())
        {
            generateTip(money, .20);
        }
        if(view.getId() == calcOther.getId())
        {
            double percent = 0;
           percent = checkForNull(otherTip, percent);
           percent /= 100;
            generateTip(money, percent);
        }

    }


    //Rounds the tip total
    public double roundTip(double tip)
    {
        tip =  Math.round(tip *100.0) / 100.0;
        Log.wtf("help", tip+"");
        return tip;
    }

    //rounds the total of the bill
    public double roundTot(double tot)
    {
        tot =  Math.round(tot);
        return tot;
    }

    //generates the total using a X% tip
    public void generateTip(double money, double percent)
    {
        double tipAmt = money * percent;
        if(tipRound.isChecked())
        {
            tipAmt= roundTip(tipAmt);
            finalAmt = money + tipAmt;

            if(totRound.isChecked()) {
                finalAmt = roundTot(finalAmt);
            }

        }
        else
        {

            finalAmt = money + tipAmt;
            finalAmt = Math.round(finalAmt *100.0) / 100.0;
            if(totRound.isChecked()) {
                finalAmt = Math.round(finalAmt);
            }
        }
        fin.setText("$" + finalAmt);
    }

    //Checks if the string is empty
    public double checkForNull(EditText txt, double value)
    {
        if(txt.getText().toString().isEmpty())
        {  value = 0;}
        else
        { value =  Double.parseDouble(txt.getText().toString());}

        return value;
    }

}
