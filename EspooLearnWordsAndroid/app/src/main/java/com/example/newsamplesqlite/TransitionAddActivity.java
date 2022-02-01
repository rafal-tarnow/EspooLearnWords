package com.example.newsamplesqlite;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputType;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.ColorUtils;

import java.util.Random;

public class TransitionAddActivity extends AppCompatActivity
{
    private EditText englishWordEditText;
    private EditText word1pronunEditText;
    private EditText foreignEditText;
    private int color;
    private Intent intent;
    private Random randomGenerator = new Random();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_transition_add);



        englishWordEditText = (EditText) findViewById(R.id.englishWord);
        word1pronunEditText = (EditText) findViewById(R.id.word1pronunEd);
        foreignEditText = (EditText) findViewById(R.id.foreignWord);
        Button add_button = (Button) findViewById(R.id.add_button);

       // custom keyboard
        MyKeyboard keyboard = (MyKeyboard) findViewById(R.id.keyboard);
        word1pronunEditText.setRawInputType(InputType.TYPE_CLASS_TEXT);
        word1pronunEditText.setTextIsSelectable(true);

        InputConnection ic = word1pronunEditText.onCreateInputConnection(new EditorInfo());
        keyboard.setInputConnection(ic);

        word1pronunEditText.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (hasFocus) {
                    //getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
                    ((InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(v.getWindowToken(), 0);
                }
            }
        });

        intent = getIntent();
        int[] colors = getResources().getIntArray(R.array.initial_colors);
        color = colors[randomGenerator.nextInt(50)];
        color = ColorUtils.setAlphaComponent(color, 90);

        englishWordEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (count == 0) {
                    // Aktualizujemy kontrolkę initialTextView.
                    //initialTextView.setText("");
                } else if (count == 1) {
                    // W kontrolce initialTextView zapisujemy pierwszą literę z kontrolki nameEditText i aktualizujemy
                    // informacje dodatkowe w intencji.
                    //initialTextView.setText(String.valueOf(s));
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });

        word1pronunEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (count == 0) {
                    // Aktualizujemy kontrolkę initialTextView.
                    //word1pronunTextView.setText("");
                } else if (count == 1) {
                    // W kontrolce initialTextView zapisujemy pierwszą literę z kontrolki nameEditText i aktualizujemy
                    // informacje dodatkowe w intencji.
                    //word1pronunTextView.setText(String.valueOf(s));
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });

        foreignEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (count == 0) {
                    // Aktualizujemy kontrolkę initialTextView.
                    //word2TextView.setText("");
                } else if (count == 1) {
                    // W kontrolce initialTextView zapisujemy pierwszą literę z kontrolki nameEditText i aktualizujemy
                    // informacje dodatkowe w intencji.
                    //word2TextView.setText(String.valueOf(s));
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });

        add_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Musi być różne od zera, w przeciwnym razie nie kończymy aktywności i wyświetlamy komunikat Toast.
                String text = String.valueOf(englishWordEditText.getText()).trim();
                if (TextUtils.isEmpty(text)) {
                    Toast.makeText(getApplicationContext(), "Podaj prawidłowe imię", Toast.LENGTH_SHORT).show();
                } else {
                    intent.putExtra(SampleMaterialActivity.EXTRA_ENGLISH_WORD, String.valueOf(englishWordEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_PRONUN, String.valueOf(word1pronunEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_FOREIGN, String.valueOf(foreignEditText.getText()));

                    intent.putExtra(SampleMaterialActivity.EXTRA_INITIAL, String.valueOf(englishWordEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_COLOR, color);
                    setResult(RESULT_OK, intent);
                    supportFinishAfterTransition();
                }
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == android.R.id.home) {
            super.onBackPressed();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
