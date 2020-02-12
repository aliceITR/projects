Program # 2

Name:Sam Blair
Cosc 4730

Description: Enter your bill into the price section. Check the boxes if you want to round the tip or the total. Select your tip percentage between 15 and 20% (the two most common tip values) or put in a custom tip. See your total at the bottom of the screen.

Ran on Google Pixel 2 API-28 and Samsung Galaxy s9 api 28

Anything that doesn't work: It only works in portrait mode and not landscape mode

# Graded: 40/50 #

*  You do not display the tip amount. **(-5 points)**

The point of the tip calculator is so that the user knows what to put for the tip, as well as what the total bill will be after the tip is included (i.e. do all the work for them).

*  Your round tip is not working. **(-5 points)**

Output is the same as the expected output if no rounding is applied. 

Test case: $37.59 with 20% tip = $7.52 tip which rounds up to $8.00 tip for total bill of $45.59.

Round Tip and Round Total Bill to nearest dollar should be mutually exclusive and you should not be able to calculate with both rounding methods applied. **(no points)**

Also, you should limit the input to 2 decimal places (i.e. not allow the user to add any more numbers after the second number has been entered after a decimal). Not worth any points, but accounting for things like this can help prevent your app from having unexpected behavior.
