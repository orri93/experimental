# Experiments with OpenAI Agents SDK

## Python Environment

Create the Virtual Environment
```bash
python -m venv venv
```
Activate the Virtual Environment
```bash
# Windows
venv\Scripts\activate
# Linux / MacOS
source venv/bin/activate
```

Install the Requirements
```bash
pip install -r requirements.txt
```

## Set Environment Variable for OpenAI API Key

```bash
# Windows
set OPENAI_API_KEY=your_openai_api_key
# Linux / MacOS
export OPENAI_API_KEY=your_openai_api_key
```

## Run the Agent

```bash
python aiagent01.py
```

## Run the Agent with Debugging

```bash
python -m debugpy --listen 5678 --wait-for-client aiagent01.py
```